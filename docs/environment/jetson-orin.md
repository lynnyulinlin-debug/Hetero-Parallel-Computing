# Jetson Orin Nano 环境

Jetson Orin Nano 是 Task 5 的第一个真实边缘硬件目标。它复用 CUDA 与 Host 调度知识，但验收对象是视频输入到端到端报告，而不是单个 kernel。

## 前置条件

- Jetson Orin Nano，已刷写匹配版本的 JetPack
- CUDA、TensorRT 与 Python 运行时可用
- 固定 TensorRT engine、视频文件和输入尺寸；4GB 首轮参数见 [实验配置](../../examples/ch14_face_detection/platforms/jetson-orin-nano/experiment-config.md)
- 可选摄像头；首个交付只要求视频文件

## 真实报告

板端适配器完成实际推理后，导出 JSON：

```bash
cd examples/ch14_face_detection
python3 pipeline.py --mode real --real-report /path/to/jetson-report.json \
  --report-json artifacts/jetson-report.json
```

报告必须记录 JetPack/CUDA/TensorRT 版本、模型、视频输入、帧数、FPS、P50/P95 延迟和失败条件。字段约定见 [Jetson 项目](../../examples/ch14_face_detection/platforms/jetson-orin-nano/README.md)。

## 边界

开发机上的 `simulated` 流水线只验证阶段和报告结构，不能代表 Jetson 性能。缺少模型、设备、运行时或真实报告时，`--mode real` 必须失败。
