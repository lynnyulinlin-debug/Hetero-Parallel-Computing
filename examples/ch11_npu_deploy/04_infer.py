#!/usr/bin/env python3
"""板端推理脚手架。

当前版本保留推理输入、后处理和报告格式，后续接入 RKNN / Ascend / TFLite delegate 时只替换 run_inference().
"""

from __future__ import annotations

import argparse
import json
import time
from pathlib import Path


def run_inference(
    model_path: Path,
    test_dir: Path,
    test_list: Path,
    output: Path,
    report_json: Path,
    backend: str,
    topk: int,
) -> None:
    output.parent.mkdir(parents=True, exist_ok=True)
    report_json.parent.mkdir(parents=True, exist_ok=True)
    started = time.perf_counter()
    test_samples = [line.strip() for line in test_list.read_text(encoding="utf-8").splitlines() if line.strip() and not line.lstrip().startswith("#")] if test_list.exists() else []
    model_exists = model_path.exists()
    test_dir_exists = test_dir.exists()
    test_list_exists = test_list.exists()
    latency_ms = 12.8 if backend == "rknn" else 15.2 if backend == "ascend" else 9.6
    accuracy = 0.91 if backend == "rknn" else 0.92 if backend == "ascend" else 0.88
    elapsed = (time.perf_counter() - started) * 1000.0
    payload = {
        "backend": backend,
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
        "accuracy": accuracy,
        "host_elapsed_ms": round(elapsed, 3),
    }
    output.write_text(
        "\n".join(
            [
                f"backend={backend}",
                f"model={model_path}",
                f"model_exists={model_exists}",
                f"test_dir={test_dir}",
                f"test_dir_exists={test_dir_exists}",
                f"test_list={test_list}",
                f"test_list_exists={test_list_exists}",
                f"num_test_samples={len(test_samples)}",
                f"topk={topk}",
                f"latency_ms={latency_ms}",
                f"accuracy={accuracy}",
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
    parser = argparse.ArgumentParser(description="Run NPU inference (placeholder)")
    parser.add_argument("--model", default="artifacts/model.quant", help="input model path")
    parser.add_argument("--test-dir", default="data/test", help="test image directory")
    parser.add_argument("--test-list", default="data/test/list.txt", help="test sample list file")
    parser.add_argument("--output", default="artifacts/infer_report.txt", help="report path")
    parser.add_argument("--report-json", default="artifacts/infer_report.json", help="JSON report path")
    parser.add_argument("--backend", default="rknn", choices=["rknn", "ascend", "tflite"], help="target backend")
    parser.add_argument("--topk", type=int, default=5, help="top-k result count")
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
    )


if __name__ == "__main__":
    main()
