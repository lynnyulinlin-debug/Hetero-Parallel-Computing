#!/usr/bin/env python3
"""分类模型推理阶段的教学脚本。"""

from __future__ import annotations

import argparse
import json
import time
from pathlib import Path


def _load_lines(path: Path) -> list[str]:
    if not path.exists():
        return []
    return [
        line.strip()
        for line in path.read_text(encoding="utf-8").splitlines()
        if line.strip() and not line.lstrip().startswith("#")
    ]


def run_inference(
    model_path: Path,
    test_dir: Path,
    test_list: Path,
    output: Path,
    report_json: Path,
    backend: str,
    topk: int,
    mode: str,
    real_report: Path | None,
) -> None:
    output.parent.mkdir(parents=True, exist_ok=True)
    report_json.parent.mkdir(parents=True, exist_ok=True)
    started = time.perf_counter()
    test_samples = _load_lines(test_list)
    model_exists = model_path.exists()
    test_dir_exists = test_dir.exists()
    test_list_exists = test_list.exists()

    if mode == "real":
        if not model_exists or not test_dir_exists or not test_list_exists or real_report is None:
            raise SystemExit("real mode requires existing model, test data and --real-report from a vendor runtime")
        if not real_report.exists():
            raise SystemExit(f"real mode report not found: {real_report}")
        measured = json.loads(real_report.read_text(encoding="utf-8"))
        required = {"latency_ms", "top1_acc", "top5_acc", "platform", "runtime_version"}
        missing = sorted(required - measured.keys())
        if missing:
            raise SystemExit(f"real mode report missing fields: {', '.join(missing)}")
        latency_ms, top1_acc, top5_acc = (measured[key] for key in ("latency_ms", "top1_acc", "top5_acc"))
        metric_source = "vendor_runtime_report"
    else:
        # Fixed values are teaching profiles, never device measurements.
        latency_ms = 10.8 if backend == "rknn" else 13.4 if backend == "ascend" else 8.9
        top1_acc = 0.915 if backend == "rknn" else 0.924 if backend == "ascend" else 0.892
        top5_acc = min(top1_acc + 0.06, 0.999)
        measured = {}
        metric_source = "teaching_profile"
    elapsed = (time.perf_counter() - started) * 1000.0
    payload = {
        "schema_version": 1,
        "execution_mode": mode,
        "metric_source": metric_source,
        "backend": backend,
        "task": "classification",
        "model": str(model_path),
        "model_exists": model_exists,
        "test_dir": str(test_dir),
        "test_dir_exists": test_dir_exists,
        "test_list": str(test_list),
        "test_list_exists": test_list_exists,
        "num_test_samples": len(test_samples),
        "samples_preview": test_samples[:3],
        "topk": topk,
        "latency_ms": latency_ms,
        "top1_acc": top1_acc,
        "top5_acc": top5_acc,
        "host_elapsed_ms": round(elapsed, 3),
        "real_report": str(real_report) if real_report else None,
        "platform": measured.get("platform"),
        "runtime_version": measured.get("runtime_version"),
    }
    output.write_text(
        "\n".join(
            [
                f"backend={backend}",
                f"execution_mode={mode}",
                f"metric_source={metric_source}",
                f"task=classification",
                f"model={model_path}",
                f"model_exists={model_exists}",
                f"test_dir={test_dir}",
                f"test_dir_exists={test_dir_exists}",
                f"test_list={test_list}",
                f"test_list_exists={test_list_exists}",
                f"num_test_samples={len(test_samples)}",
                f"topk={topk}",
                f"latency_ms={latency_ms}",
                f"top1_acc={top1_acc}",
                f"top5_acc={top5_acc}",
            ]
        )
        + "\n",
        encoding="utf-8",
    )
    report_json.write_text(json.dumps(payload, indent=2, ensure_ascii=False), encoding="utf-8")
    print(f"[infer] wrote inference summary to {output}")
    print(f"[infer] wrote inference report to {report_json}")
    if not model_exists:
        print(f"[infer] warning: model file not found: {model_path}")
    if not test_dir_exists:
        print(f"[infer] warning: test directory not found: {test_dir}")
    if not test_list_exists:
        print(f"[infer] warning: test list not found: {test_list}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Run classification inference (teaching artifact)")
    parser.add_argument("--model", default="artifacts/model.quant", help="input model path")
    parser.add_argument("--test-dir", default="data/test", help="test image directory")
    parser.add_argument("--test-list", default="data/test/list.txt", help="test sample list file")
    parser.add_argument("--output", default="artifacts/infer_report.txt", help="report path")
    parser.add_argument("--report-json", default="artifacts/infer_report.json", help="JSON report path")
    parser.add_argument("--backend", default="rknn", choices=["rknn", "ascend", "tflite"], help="target backend")
    parser.add_argument("--topk", type=int, default=5, help="top-k result count")
    parser.add_argument("--mode", default="simulated", choices=["simulated", "real"], help="report provenance")
    parser.add_argument("--real-report", default="", help="vendor runtime JSON for real mode")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    run_inference(
        Path(args.model),
        Path(args.test_dir),
        Path(args.test_list),
        Path(args.output),
        Path(args.report_json),
        args.backend,
        args.topk,
        args.mode,
        Path(args.real_report) if args.real_report else None,
    )


if __name__ == "__main__":
    main()
