"""CPU capture/decode stage (simulated).

支持四种输入源：
- simulated：生成固定长度的模拟帧序列
- list：从文本列表读取帧来源路径，用于后续接入真实视频文件
- camera：摄像头来源标识
- stream：网络视频流来源标识
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
    sources = [line.strip() for line in path.read_text(encoding="utf-8").splitlines() if line.strip() and not line.lstrip().startswith("#")]
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
