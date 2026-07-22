# 流媒体输入样例

这个样例用于把真实视频文件、摄像头和 RTSP 流统一写进 `source.list`。
当前流水线不会真正拉流解码，但来源标签和目录约定已经固定下来，后续替换解码器时不需要改接口。

## 1. 推荐写法

```text
# 本地文件
samples/demo_0001.mp4
samples/demo_0002.mp4

# 摄像头
camera:0

# RTSP 流
rtsp://192.168.1.10:554/stream1
rtsp://192.168.1.11:554/stream2
```

## 2. 使用方式

把上面的内容保存到 `models/source.list`，然后运行：

```bash
python3 pipeline.py \
  --backend gpu \
  --source list \
  --input-list models/source.list \
  --frames 3
```

## 3. 当前版本的行为

- `.mp4` / `.mkv` / `.avi` / `.mov` 会被标记为 `video`
- `camera:*` 会被标记为 `camera`
- `rtsp://...` 会被标记为 `rtsp`
- 如果 `source.list` 不存在或为空，流水线会回退到模拟帧

## 4. 接入提醒

- 真实工程里通常先把 RTSP 拉流和摄像头输入统一封装成“来源字符串”，再在解码层做具体映射。
- 后续接真实解码器时，优先保持 `pipeline.py` 的 CLI 不变，只替换 `stages/capture_decode.py`。
