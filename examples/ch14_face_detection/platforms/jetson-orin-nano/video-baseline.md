# Jetson 固定视频验收基线

首轮 Jetson 验收使用**单个固定视频文件**。它用于比较同一模型、同一运行时和同一改动前后的结果；摄像头、RTSP 和图片不作为第一份性能报告的输入。

## 输入层次

| 层次 | 用途 | 是否可用于首份性能报告 |
|---|---|---|
| 单张图片 | 验证模型输入、预处理、检测框和 TensorRT engine 能否工作 | 否 |
| 固定视频文件 | 测量解码、推理、后处理、队列和端到端延迟 | 是 |
| 摄像头 / RTSP | 验证实时输入稳定性、丢帧和网络波动 | 否，属于后续扩展 |

## 视频文件要求

- 使用本地可读取的视频文件，例如 MP4；不把网络拉流结果混入首轮基线。
- 记录视频路径或稳定标识、SHA-256、分辨率、帧率和实际测量帧数。
- 预热帧与统计帧分开记录。`frames` 必须等于 `measured_frames`，不包含预热帧。
- 同一次对照中固定模型、engine、功耗模式、JetPack/CUDA/TensorRT 版本和视频文件。

仓库不提交媒体素材或模型权重。设备持有者自行准备合法可分发的视频；报告保留哈希而非提交视频本体。

## 图片预检

先用一张静态图片确认 TensorRT engine、输入尺寸、预处理和检测输出没有明显错误。预检只回答“能否正确推理”，不记录 FPS、P50 或 P95，不得替代视频报告。

## 报告字段

`pipeline.py --mode real` 默认采用 `video-baseline` 策略，并要求外部设备适配器生成下列字段：

```json
{
  "execution_mode": "real",
  "platform": "Jetson Orin Nano",
  "runtime_version": "JetPack <version>; CUDA <version>; TensorRT <version>",
  "model": "models/face.engine",
  "input_source": "samples/baseline.mp4",
  "input_kind": "video_file",
  "input_sha256": "<64 lowercase hexadecimal characters>",
  "input_width": 1280,
  "input_height": 720,
  "warmup_frames": 30,
  "frames": 300,
  "measured_frames": 300,
  "fps": 24.8,
  "latency_ms_p50": 37.9,
  "latency_ms_p95": 46.2
}
```

示例数值只说明字段形状，不是 Jetson 性能承诺。执行归档时使用：

```bash
python3 pipeline.py --mode real \
  --real-report /path/to/jetson-video-report.json \
  --report-json artifacts/jetson-video-report.json
```

摄像头或 RTSP 报告需要先明确放宽输入策略，且必须在报告中保留 `input_kind`：

```bash
python3 pipeline.py --mode real --real-input-policy any \
  --real-report /path/to/jetson-camera-report.json
```

这类报告不能和固定视频基线直接比较。
