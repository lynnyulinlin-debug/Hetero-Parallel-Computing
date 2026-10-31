#!/usr/bin/env python3
"""分类模型导出阶段的教学脚本。

这一层固定模型名、输入输出张量和预处理约定，用来说明导出阶段真正要冻结什么。
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def export_model(
    model_name: str,
    checkpoint: Path | None,
    output: Path,
    report: Path,
    input_shape: list[int],
    input_name: str,
    output_name: str,
    opset: int,
    dynamic_batch: bool,
    mean: list[float],
    std: list[float],
    class_count: int,
) -> None:
    output.parent.mkdir(parents=True, exist_ok=True)
    report.parent.mkdir(parents=True, exist_ok=True)
    checkpoint_exists = checkpoint.exists() if checkpoint else False

    artifact = {
        "format": "onnx",
        "task": "classification",
        "model": model_name,
        "class_count": class_count,
        "checkpoint": str(checkpoint) if checkpoint else None,
        "checkpoint_exists": checkpoint_exists,
        "input_name": input_name,
        "output_name": output_name,
        "input_shape": input_shape,
        "dynamic_batch": dynamic_batch,
        "opset": opset,
        "normalization": {"mean": mean, "std": std},
    }
    output.write_text(json.dumps(artifact, indent=2, ensure_ascii=False), encoding="utf-8")
    report.write_text(
        json.dumps(
            {
                "status": "ok",
                "artifact": str(output),
                "checkpoint_exists": checkpoint_exists,
                "task": "classification",
            },
            indent=2,
            ensure_ascii=False,
        ),
        encoding="utf-8",
    )
    print(f"[export] wrote ONNX artifact manifest to {output}")
    print(f"[export] wrote export report to {report}")
    if checkpoint and not checkpoint_exists:
        print(f"[export] warning: checkpoint not found: {checkpoint}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Export a classification model to ONNX (teaching manifest)")
    parser.add_argument("--model", default="resnet18", help="model name or checkpoint id")
    parser.add_argument("--checkpoint", default="", help="optional checkpoint path")
    parser.add_argument("--output", default="artifacts/model.onnx", help="output ONNX path")
    parser.add_argument("--report", default="artifacts/export_report.json", help="export report path")
    parser.add_argument("--input-shape", default="1,3,224,224", help="N,C,H,W input shape")
    parser.add_argument("--input-name", default="input", help="input tensor name")
    parser.add_argument("--output-name", default="logits", help="output tensor name")
    parser.add_argument("--opset", type=int, default=17, help="ONNX opset version")
    parser.add_argument("--dynamic-batch", action="store_true", help="allow dynamic batch axis")
    parser.add_argument("--mean", default="0.485,0.456,0.406", help="normalization mean")
    parser.add_argument("--std", default="0.229,0.224,0.225", help="normalization std")
    parser.add_argument("--class-count", type=int, default=1000, help="number of classes")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    checkpoint = Path(args.checkpoint) if args.checkpoint else None
    input_shape = [int(v) for v in args.input_shape.split(",") if v.strip()]
    mean = [float(v) for v in args.mean.split(",") if v.strip()]
    std = [float(v) for v in args.std.split(",") if v.strip()]
    if len(input_shape) != 4:
        raise SystemExit("--input-shape must contain 4 values: N,C,H,W")
    if len(mean) != 3 or len(std) != 3:
        raise SystemExit("--mean and --std must each contain 3 values")
    export_model(
        args.model,
        checkpoint,
        Path(args.output),
        Path(args.report),
        input_shape,
        args.input_name,
        args.output_name,
        args.opset,
        args.dynamic_batch,
        mean,
        std,
        args.class_count,
    )


if __name__ == "__main__":
    main()
