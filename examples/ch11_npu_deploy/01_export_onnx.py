#!/usr/bin/env python3
"""导出模型到 ONNX 的脚手架。

当前版本不依赖 PyTorch，先把真实工程需要的导出参数、工件布局和报告格式固定下来。
后续接入真实模型时，只需要替换 export_model() 内部实现。
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
) -> None:
    output.parent.mkdir(parents=True, exist_ok=True)
    report.parent.mkdir(parents=True, exist_ok=True)
    checkpoint_exists = checkpoint.exists() if checkpoint else False

    artifact = {
        "format": "onnx",
        "model": model_name,
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
        json.dumps({"status": "ok", "artifact": str(output), "checkpoint_exists": checkpoint_exists}, indent=2),
        encoding="utf-8",
    )
    print(f"[export] wrote ONNX artifact manifest to {output}")
    print(f"[export] wrote export report to {report}")
    if checkpoint and not checkpoint_exists:
        print(f"[export] warning: checkpoint not found: {checkpoint}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Export a model to ONNX (placeholder)")
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
    )


if __name__ == "__main__":
    main()
