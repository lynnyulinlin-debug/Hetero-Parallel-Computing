"""GPU 推理后端（教学版）。"""

from __future__ import annotations

import time


def infer(frame):
    # GPU 版本只模拟更低的单帧耗时，便于观察后端切换对流水线的影响。
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
