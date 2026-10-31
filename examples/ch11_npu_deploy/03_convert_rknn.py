#!/usr/bin/env python3
"""转换阶段的教学脚本。

这一层只演示“平台相关转换入口长什么样”，方便把 ONNX 中间表示和厂商专有格式分开理解。
"""

from __future__ import annotations

import argparse
from pathlib import Path


def convert(onnx_path: Path, output: Path) -> None:
    output.parent.mkdir(parents=True, exist_ok=True)
    # 这里只冻结转换结果的路径与来源关系，真实转换由厂商工具接管。
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
