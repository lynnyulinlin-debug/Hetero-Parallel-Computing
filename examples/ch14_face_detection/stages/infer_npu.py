"""NPU inference backend (simulated)."""

from __future__ import annotations

import time


def infer(frame):
    time.sleep(0.0015)
    return [
        {
            "label": "face",
            "score": 0.91,
            "bbox": [103, 79, 181, 183],
            "backend": "npu",
            "frame_id": frame["id"],
        }
    ]
