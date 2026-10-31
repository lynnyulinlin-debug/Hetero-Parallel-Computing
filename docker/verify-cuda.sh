#!/usr/bin/env bash
set -euo pipefail

# CPU and contract tests are portable; CUDA tests require a device passed by the host.
ctest --test-dir build -L 'cpu|python' --output-on-failure
ctest --test-dir build -L cuda --output-on-failure
