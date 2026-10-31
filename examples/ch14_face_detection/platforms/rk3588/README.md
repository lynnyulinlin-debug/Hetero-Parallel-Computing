# RK3588 端侧运行契约

本目录保留 RK3588 作为 Jetson Orin Nano 之后的 NPU 对照平台。仓库不附带 RKNN Toolkit、板端运行时、模型权重或真实媒体；这些输入必须由实际设备环境提供。

## 真实模式输入

运行时适配器完成一次真实视频推理后，应产生 JSON 报告，并至少包含：

- `platform`：例如 `RK3588`。
- `runtime_version`：RKNN Runtime 与系统版本。
- `model`、`input_source`、`frames`：模型和输入身份。
- `fps`、`latency_ms_p50`、`latency_ms_p95`：端到端测量口径。
- `execution_mode`：固定为 `real`。

将该报告传给：

```bash
python3 pipeline.py --mode real --real-report /path/to/rk3588-report.json
```

真实模式只校验并转存已有的板端测量；它不会在开发机上模拟出 RK3588 结果。

## 接入顺序

1. 在板端用 RKNN Toolkit / Runtime 转换并加载一个固定模型。
2. 将视频文件或摄像头帧送入真实解码器与推理适配器。
3. 记录平台、运行时、模型、输入、帧数和延迟分位数。
4. 导出符合上述字段的 JSON，再交给本项目归档。
