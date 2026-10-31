#!/usr/bin/env python3
"""实时视频流人脸检测的教学流水线。

这条流水线的重点不是单个模型，而是把采集、推理和后处理之间的阶段边界固定下来。
"""

from __future__ import annotations

import argparse
import json
import queue
import threading
import time
from collections import defaultdict
from pathlib import Path

from stages.capture_decode import capture_frames
from stages.infer_cpu import infer as infer_cpu
from stages.infer_gpu import infer as infer_gpu
from stages.infer_npu import infer as infer_npu
from stages.postprocess import postprocess_frame


def choose_backend(name: str):
    backends = {
        "cpu": infer_cpu,
        "gpu": infer_gpu,
        "npu": infer_npu,
    }
    return backends[name]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="End-to-end face detection pipeline")
    parser.add_argument("--backend", choices=["cpu", "gpu", "npu"], default="cpu")
    parser.add_argument("--frames", type=int, default=5)
    parser.add_argument("--queue-size", type=int, default=2, help="bounded queue size between stages")
    parser.add_argument(
        "--source",
        choices=["simulated", "list", "camera", "stream"],
        default="simulated",
        help="frame source type",
    )
    parser.add_argument("--input-list", default="", help="text file with one source per line")
    parser.add_argument("--mode", choices=["simulated", "real"], default="simulated", help="measurement provenance")
    parser.add_argument("--real-report", default="", help="vendor runtime JSON for real mode")
    parser.add_argument(
        "--real-input-policy",
        choices=["video-baseline", "any"],
        default="video-baseline",
        help="require a fixed video file for real measurements unless explicitly relaxed",
    )
    parser.add_argument("--report-json", default="", help="write pipeline report JSON")
    return parser.parse_args()


def validate_real_report(payload: dict, input_policy: str) -> list[str]:
    required = {
        "execution_mode",
        "experiment_config_id",
        "platform",
        "runtime_version",
        "model",
        "input_source",
        "input_kind",
        "input_sha256",
        "input_width",
        "input_height",
        "frames",
        "warmup_frames",
        "measured_frames",
        "memory_peak_mb",
        "swap_peak_mb",
        "fps",
        "latency_ms_p50",
        "latency_ms_p95",
    }
    missing = sorted(required - payload.keys())
    errors = [f"missing: {', '.join(missing)}"] if missing else []
    if payload.get("execution_mode") != "real":
        errors.append("execution_mode must be real")
    if not isinstance(payload.get("experiment_config_id"), str) or not payload["experiment_config_id"]:
        errors.append("experiment_config_id must identify the measurement configuration")
    if input_policy == "video-baseline" and payload.get("input_kind") != "video_file":
        errors.append("video-baseline requires input_kind=video_file")
    for field in ("input_width", "input_height", "frames", "measured_frames"):
        if field in payload and (not isinstance(payload[field], int) or payload[field] <= 0):
            errors.append(f"{field} must be a positive integer")
    for field in ("memory_peak_mb", "swap_peak_mb"):
        if field in payload and (not isinstance(payload[field], int) or payload[field] < 0):
            errors.append(f"{field} must be a non-negative integer")
    if "warmup_frames" in payload and (not isinstance(payload["warmup_frames"], int) or payload["warmup_frames"] < 0):
        errors.append("warmup_frames must be a non-negative integer")
    for field in ("fps", "latency_ms_p50", "latency_ms_p95"):
        if field in payload and (not isinstance(payload[field], (int, float)) or payload[field] <= 0):
            errors.append(f"{field} must be positive")
    if payload.get("latency_ms_p95", 0) < payload.get("latency_ms_p50", 0):
        errors.append("latency_ms_p95 must be >= latency_ms_p50")
    if payload.get("frames") != payload.get("measured_frames"):
        errors.append("frames must equal measured_frames")
    digest = payload.get("input_sha256")
    if not isinstance(digest, str) or len(digest) != 64 or any(char not in "0123456789abcdef" for char in digest):
        errors.append("input_sha256 must be a lowercase SHA-256 hex string")
    return errors


def main() -> None:
    args = parse_args()
    if args.mode == "real":
        if not args.real_report:
            raise SystemExit("real mode requires --real-report from a device runtime adapter")
        report_path = Path(args.real_report)
        if not report_path.exists():
            raise SystemExit(f"real mode report not found: {report_path}")
        payload = json.loads(report_path.read_text(encoding="utf-8"))
        errors = validate_real_report(payload, args.real_input_policy)
        if errors:
            raise SystemExit(f"invalid real report; {'; '.join(errors)}")
        if args.report_json:
            Path(args.report_json).write_text(json.dumps(payload, indent=2, ensure_ascii=False), encoding="utf-8")
        print(f"[pipeline] execution_mode=real platform={payload['platform']} fps={payload['fps']}")
        return
    infer = choose_backend(args.backend)

    # 先把帧流、阶段队列和停止信号组织成可观察的最小流水线。
    frames = capture_frames(args.frames, source=args.source, input_list=args.input_list or None)
    capture_q: queue.Queue = queue.Queue(maxsize=args.queue_size)
    infer_q: queue.Queue = queue.Queue(maxsize=args.queue_size)
    stop = object()
    results = []
    stats = defaultdict(list)
    queue_max = {"capture_q": 0, "infer_q": 0}

    def track(stage: str, start: float) -> None:
        stats[stage].append((time.perf_counter() - start) * 1000.0)

    def update_max(name: str, q: queue.Queue) -> None:
        queue_max[name] = max(queue_max[name], q.qsize())

    def producer() -> None:
        for frame in frames:
            t0 = time.perf_counter()
            capture_q.put(frame)
            track("capture_ms", t0)
            update_max("capture_q", capture_q)
        # 用显式停止标记收口，避免消费者线程靠隐式条件退出。
        capture_q.put(stop)

    def infer_worker() -> None:
        while True:
            frame = capture_q.get()
            if frame is stop:
                infer_q.put(stop)
                capture_q.task_done()
                break
            t0 = time.perf_counter()
            detections = infer(frame)
            track("infer_ms", t0)
            infer_q.put((frame, detections))
            update_max("infer_q", infer_q)
            capture_q.task_done()

    def post_worker() -> None:
        while True:
            item = infer_q.get()
            if item is stop:
                infer_q.task_done()
                break
            frame, detections = item
            t0 = time.perf_counter()
            results.append(postprocess_frame(frame, detections))
            track("post_ms", t0)
            infer_q.task_done()

    t0 = time.perf_counter()
    threads = [
        threading.Thread(target=producer, name="producer"),
        threading.Thread(target=infer_worker, name="infer"),
        threading.Thread(target=post_worker, name="post"),
    ]
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join()
    elapsed = time.perf_counter() - t0
    fps = len(frames) / elapsed if elapsed > 0 else 0.0

    def avg_ms(name: str) -> float:
        values = stats.get(name, [])
        return (sum(values) / len(values)) if values else 0.0

    print(
        f"[pipeline] backend={args.backend} source={args.source} frames={len(frames)} elapsed={elapsed:.3f}s "
        f"fps={fps:.2f} capture_q_max={queue_max['capture_q']} infer_q_max={queue_max['infer_q']}"
    )
    print(
        f"[pipeline] stage_ms capture={avg_ms('capture_ms'):.3f} "
        f"infer={avg_ms('infer_ms'):.3f} post={avg_ms('post_ms'):.3f}"
    )
    for item in results:
        print(item)
    if args.report_json:
        payload = {
            "schema_version": 1,
            "execution_mode": "simulated",
            "metric_source": "teaching_pipeline",
            "backend": args.backend,
            "source": args.source,
            "frames": len(frames),
            "fps": round(fps, 3),
            "stage_ms": {name: round(avg_ms(name), 3) for name in ("capture_ms", "infer_ms", "post_ms")},
            "queue_max": queue_max,
        }
        Path(args.report_json).write_text(json.dumps(payload, indent=2, ensure_ascii=False), encoding="utf-8")


if __name__ == "__main__":
    main()
