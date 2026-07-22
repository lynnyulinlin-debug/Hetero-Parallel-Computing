"""CPU postprocess stage (simulated NMS + render)."""

from __future__ import annotations

import time


def postprocess_frame(frame, detections):
    time.sleep(0.0007)
    best = max(detections, key=lambda det: det["score"])
    x1, y1, x2, y2 = best["bbox"]
    return (
        f"[frame {frame['id']}] backend={best['backend']} "
        f"face score={best['score']:.2f} bbox=({x1},{y1},{x2},{y2})"
    )
