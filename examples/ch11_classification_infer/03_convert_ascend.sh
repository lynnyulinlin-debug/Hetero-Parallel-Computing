#!/usr/bin/env bash
set -euo pipefail

ONNX_PATH="${1:-artifacts/model.onnx}"
OUT_PATH="${2:-artifacts/model.om}"

mkdir -p "$(dirname "$OUT_PATH")"
printf '# teaching Ascend artifact\nkind=classification\nonnx=%s\n' "$ONNX_PATH" > "$OUT_PATH"
printf '[convert-ascend] wrote teaching artifact to %s\n' "$OUT_PATH"
