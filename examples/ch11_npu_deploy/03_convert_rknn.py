#!/usr/bin/env python3
"""ONNX 转 RKNN 的占位脚本。"""

from __future__ import annotations

import argparse
from pathlib import Path


def convert(onnx_path: Path, output: Path) -> None:
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(f"# placeholder RKNN artifact\nonnx={onnx_path}\n", encoding="utf-8")
    print(f"[convert-rknn] wrote placeholder artifact to {output}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Convert ONNX to RKNN (placeholder)")
    parser.add_argument("--onnx", default="artifacts/model.onnx", help="input ONNX path")
    parser.add_argument("--output", default="artifacts/model.rknn", help="output RKNN path")
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    convert(Path(args.onnx), Path(args.output))


if __name__ == "__main__":
    main()
