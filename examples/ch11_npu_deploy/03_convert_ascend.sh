#!/usr/bin/env bash
set -euo pipefail

ONNX_PATH="${1:-artifacts/model.onnx}"
OUT_PATH="${2:-artifacts/model.om}"

mkdir -p "$(dirname "$OUT_PATH")"
printf '# placeholder Ascend artifact\nonnx=%s\n' "$ONNX_PATH" > "$OUT_PATH"
printf '[convert-ascend] wrote placeholder artifact to %s\n' "$OUT_PATH"
