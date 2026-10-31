"""采集与解码阶段（教学版）。

这里把输入源规范成四类，目的是让读者先看懂“帧从哪里来”，再接入真实视频或摄像头。
"""

from __future__ import annotations

from pathlib import Path


def _load_source_list(input_list: str | None):
    if not input_list:
        return []
    path = Path(input_list)
    if not path.exists():
        print(f"[capture] source list not found: {path}; falling back to simulated frames")
        return []
    sources = [
        line.strip()
        for line in path.read_text(encoding="utf-8").splitlines()
        if line.strip() and not line.lstrip().startswith("#")
    ]
    if not sources:
        print(f"[capture] source list is empty: {path}; falling back to simulated frames")
    return sources


def capture_frames(num_frames: int, source: str = "simulated", input_list: str | None = None):
    sources = _load_source_list(input_list)
    frames = []
    for idx in range(num_frames):
        if sources:
            source_item = sources[idx % len(sources)]
        elif source == "camera":
            source_item = "camera:0"
        elif source == "stream":
            source_item = "stream:0"
        else:
            source_item = f"frame-{idx}"
        # 这里把来源类型归一化成便于观察的标签，方便后面统计不同输入源。
        source_kind = source
        if source_item.startswith("camera:"):
            source_kind = "camera"
        elif source_item.startswith("stream:"):
            source_kind = "stream"
        elif source_item.startswith("rtsp://"):
            source_kind = "rtsp"
        elif source_item.startswith(("http://", "https://")):
            source_kind = "http"
        elif source_item.endswith((".mp4", ".mkv", ".avi", ".mov")):
            source_kind = "video"
        frames.append(
            {
                "id": idx,
                "width": 640,
                "height": 360,
                "timestamp_ms": idx * 33,
                "source": source_kind,
                "source_item": source_item,
                "pixels": f"frame-{idx}",
            }
        )
    return frames
