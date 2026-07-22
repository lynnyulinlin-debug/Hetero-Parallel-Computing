#!/usr/bin/env python3
"""实时视频流人脸检测流水线骨架。

当前版本用模拟帧和模拟检测结果把数据流、后端切换和阶段衔接固定下来。
后续接入真实模型时，只替换 stages/*.py 的实现即可。
"""

from __future__ import annotations

import argparse
import queue
import threading
import time
from collections import defaultdict

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
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    infer = choose_backend(args.backend)

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


if __name__ == "__main__":
    main()
