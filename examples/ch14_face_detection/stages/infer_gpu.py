"""GPU inference backend (simulated)."""

from __future__ import annotations

import time


def infer(frame):
    time.sleep(0.002)
    return [
        {
            "label": "face",
            "score": 0.93,
            "bbox": [104, 78, 182, 184],
            "backend": "gpu",
            "frame_id": frame["id"],
        }
    ]
