"""CPU inference backend (simulated baseline)."""

from __future__ import annotations

import time


def infer(frame):
    time.sleep(0.004)
    return [
        {
            "label": "face",
            "score": 0.88,
            "bbox": [100, 80, 180, 180],
            "backend": "cpu",
            "frame_id": frame["id"],
        }
    ]
