"""NPU 推理后端（教学版）。"""

from __future__ import annotations

import time


def infer(frame):
    # NPU 版本模拟更快但更专用的推理路径，适合和 CPU / GPU 版本对照。
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
