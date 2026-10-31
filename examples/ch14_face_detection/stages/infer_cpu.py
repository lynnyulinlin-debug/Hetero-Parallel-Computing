"""CPU 推理后端（教学基线）。"""

from __future__ import annotations

import time


def infer(frame):
    # CPU 版本作为对照基线，延迟略高但接口最简单。
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
