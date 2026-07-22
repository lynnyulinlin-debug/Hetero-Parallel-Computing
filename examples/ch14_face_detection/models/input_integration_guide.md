# 真实输入流接入指南

这份指南说明如何把真实输入接入第14章的 `pipeline.py`。
目标是先把输入源、来源标签和回退行为固定下来，再替换真实解码器。

## 1. 选择输入模式

- `--source simulated`：本地无硬件时的默认模式。
- `--source list`：批量来源模式，适合文件、摄像头、RTSP 混合写法。
- `--source camera`：单摄像头模式。
- `--source stream`：单网络流模式。

## 2. 推荐来源写法

`models/source.list` 可以写成下面这样：

```text
# 本地视频
samples/demo_0001.mp4
samples/demo_0002.mp4

# 摄像头
camera:0

# RTSP
rtsp://192.168.1.10:554/stream1
```

## 3. 目录边界

```text
ch14_face_detection/
├── models/
│   ├── source.list
│   ├── video_source_example.md
│   ├── camera_source_example.md
│   └── stream_source_example.md
└── stages/capture_decode.py
```

- `models/source.list` 只存来源字符串。
- `stages/capture_decode.py` 只负责把来源映射成帧输入。
- `pipeline.py` 的 CLI 尽量不改。

## 4. 当前代码的行为

- `camera:*` 会标记成摄像头来源。
- `stream:*`、`rtsp://...`、`http(s)://...` 会标记成流媒体来源。
- `*.mp4`、`*.mkv`、`*.avi`、`*.mov` 会标记成视频来源。
- `source.list` 不存在或为空时，会回退到模拟帧。

## 5. 接入真实解码器时怎么改

1. 先保留 `pipeline.py` 的参数和输出格式。
2. 再替换 `stages/capture_decode.py` 里的帧生成逻辑。
3. 如果需要，最后再把 `source` 标签映射到真实设备或拉流句柄。

## 6. 最后确认

接入完成后，至少要能回答这三个问题：

- 输入源从哪里来？
- 来源标签怎么写？
- 没有真实硬件时怎么回退？
