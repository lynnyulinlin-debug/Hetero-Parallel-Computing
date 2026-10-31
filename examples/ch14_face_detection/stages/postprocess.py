"""CPU 后处理阶段（教学版）。"""

from __future__ import annotations

import time


def postprocess_frame(frame, detections):
    # 后处理阶段负责把模型输出转成最终可展示结果，是流水线的收口位置。
    time.sleep(0.0007)
    best = max(detections, key=lambda det: det["score"])
    x1, y1, x2, y2 = best["bbox"]
    return (
        f"[frame {frame['id']}] backend={best['backend']} "
        f"face score={best['score']:.2f} bbox=({x1},{y1},{x2},{y2})"
    )
