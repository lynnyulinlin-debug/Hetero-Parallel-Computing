#!/usr/bin/env python3
"""NPU PTQ 量化脚手架。

当前版本不依赖厂商 SDK，先把校准集、后端选择和量化报告格式固定下来。
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path


def _load_lines(path: Path) -> list[str]:
    if not path.exists():
        return []
    return [
        line.strip()
        for line in path.read_text(encoding="utf-8").splitlines()
        if line.strip() and not line.lstrip().startswith("#")
    ]


def quantize_model(
    onnx_path: Path,
    calib_dir: Path,
    calib_list: Path,
    output: Path,
    report: Path,
    backend: str,
    scheme: str,
    bit_width: int,
) -> None:
    output.parent.mkdir(parents=True, exist_ok=True)
    report.parent.mkdir(parents=True, exist_ok=True)
    samples = _load_lines(calib_list)
    onnx_exists = onnx_path.exists()
    calib_dir_exists = calib_dir.exists()
    calib_list_exists = calib_list.exists()

    artifact = {
        "format": "quantized",
        "backend": backend,
        "scheme": scheme,
        "bit_width": bit_width,
        "source_onnx": str(onnx_path),
        "source_onnx_exists": onnx_exists,
        "calibration_dir": str(calib_dir),
        "calibration_dir_exists": calib_dir_exists,
        "calibration_list": str(calib_list),
        "calibration_list_exists": calib_list_exists,
        "num_calibration_samples": len(samples),
        "samples_preview": samples[:3],
    }
    output.write_text(json.dumps(artifact, indent=2, ensure_ascii=False), encoding="utf-8")
    report.write_text(
        json.dumps(
            {
                "status": "ok",
                "artifact": str(output),
                "num_calibration_samples": len(samples),
                "source_onnx_exists": onnx_exists,
                "calibration_dir_exists": calib_dir_exists,
                "calibration_list_exists": calib_list_exists,
            },
            indent=2,
        ),
        encoding="utf-8",
    )
    print(f"[quantize] wrote quantized artifact manifest to {output}")
    print(f"[quantize] wrote quantization report to {report}")
    if not onnx_exists:
        print(f"[quantize] warning: ONNX file not found: {onnx_path}")
    if not calib_dir_exists:
        print(f"[quantize] warning: calibration directory not found: {calib_dir}")
    if not calib_list_exists:
        print(f"[quantize] warning: calibration list not found: {calib_list}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Quantize an ONNX model (placeholder)")
    parser.add_argument("--onnx", default="artifacts/model.onnx", help="input ONNX path")
    parser.add_argument("--calib-dir", default="data/calib", help="calibration data directory")
    parser.add_argument("--calib-list", default="data/calib/list.txt", help="calibration list file")
    parser.add_argument("--output", default="artifacts/model.quant", help="output quantized path")
    parser.add_argument("--report", default="artifacts/quant_report.json", help="quantization report path")
    parser.add_argument("--backend", default="rknn", choices=["rknn", "ascend", "tflite"], help="target backend")
    parser.add_argument("--scheme", default="ptq", choices=["ptq", "qat"], help="quantization scheme")
    parser.add_argument("--bit-width", type=int, default=8, help="quantization bit width")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    quantize_model(
        Path(args.onnx),
        Path(args.calib_dir),
        Path(args.calib_list),
        Path(args.output),
        Path(args.report),
        args.backend,
        args.scheme,
        args.bit_width,
    )


if __name__ == "__main__":
    main()
