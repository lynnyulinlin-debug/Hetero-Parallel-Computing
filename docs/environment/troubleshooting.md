# 环境排障

| 现象 | 先检查什么 |
|---|---|
| `cmake` 找不到 OpenMP | 编译器版本与 OpenMP 支持；先验证 CPU CTest |
| `nvcc` 不存在 | CUDA Toolkit 是否安装并在 `PATH` 中 |
| CUDA target 构建成功但测试跳过 | 驱动、GPU 可见性和 `cudaGetDeviceCount` |
| Jetson real 模式失败 | TensorRT engine、视频输入、设备报告字段与运行时版本 |
| NPU real 模式失败 | 模型、校准数据、厂商 SDK、设备连接和报告字段 |

排障时先保留失败命令、平台版本、输入规模和完整错误信息。不要用模拟报告覆盖真实设备失败。
