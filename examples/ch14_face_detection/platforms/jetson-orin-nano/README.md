# Jetson Orin Nano 端侧视觉项目

`ch14_face_detection` 的第一个真实硬件目标是 Jetson Orin Nano。它复用第7章的 Host 调度、第9章的 CUDA 执行模型和第10章的测量方法，把本地算子实验推进为边缘视频推理。

## 交付范围

```text
video file or camera
    -> decode / preprocess
    -> TensorRT inference on Jetson GPU
    -> CPU postprocess
    -> JSON measurement report
```

首个版本只要求固定视频文件输入；单张图片只用于模型正确性预检，摄像头、RTSP、显示叠框和功耗采样都属于后续扩展。具体输入与报告字段见 [固定视频验收基线](video-baseline.md)，4GB 设备的首轮参数见 [实验配置](experiment-config.md)。

## 真实模式报告

板端适配器完成实际推理后，导出 JSON 并交给统一流水线归档：

```bash
python3 pipeline.py --mode real --real-report /path/to/jetson-report.json \
  --report-json artifacts/jetson-report.json
```

报告必须包含：

- `execution_mode: "real"`
- `platform: "Jetson Orin Nano"`
- `runtime_version`：JetPack、CUDA、TensorRT 版本
- `model`、`input_source`、`input_kind: "video_file"`、视频 SHA-256 和分辨率
- `warmup_frames`、`frames`、`measured_frames`
- `fps`、`latency_ms_p50`、`latency_ms_p95`

真实模式只归档已有板端测量，不能在开发机上生成 Jetson 性能数字。

## 验收记录

每次测量至少记录模型输入尺寸、视频分辨率、帧数、功耗模式、运行时版本和失败条件。先与 `--mode simulated` 的阶段结构对照，再分析真实结果中解码、推理、后处理和队列哪一段限制端到端吞吐。
