# 附录D 常见错误与解决方法（FAQ）

> 按章节积累的高频坑，v1.0 持续补全。

## 环境与构建

- **Q: `make` 提示找不到 `nvcc`？** 你在构建 CPU-only 目标无需 nvcc；构建 CUDA 版需安装 CUDA Toolkit 并确保 `nvcc` 在 PATH。
- **Q: OpenMP 没生效（单线程）？** 确认编译带 `-fopenmp`，运行时用 `OMP_NUM_THREADS` 设置线程数。

## CUDA

- **Q: 核函数"没运行"也不报错？** CUDA 启动是异步的，需用 `cudaGetLastError()` + `cudaDeviceSynchronize()` 捕获错误（见 `common/cuda_helpers.cuh` 的 `CUDA_CHECK_KERNEL`）。
- **Q: 结果全是 0 或随机值？** 多半忘了 H2D/D2H 拷贝，或核函数边界检查缺失导致越界。
- **Q: 加了 GPU 反而更慢？** 检查是否带宽受限或被传输开销主导（见第8章算术强度、第9.4节向量加法教训）。
- **Q: `USE_CUDA=1` 编译成功，但运行提示没有 CUDA-capable device？** 这说明机器上没有可用 NVIDIA GPU；`examples/ch09_reduce` 会自动跳过 CUDA 测试并保留 CPU 结果。

## NPU 部署

- **Q: 算子不支持？** 检查厂商算子支持列表，考虑替换算子或回退 CPU 执行。
- **Q: 量化后精度大幅下降？** 检查校准集是否有代表性，或改用 QAT（见第11.4节）。
- **Q: 动态 shape 报错？** 多数 NPU 需固定输入尺寸，导出 ONNX 时固定 batch/分辨率。
- **Q: 只有脚手架、还没接真实模型？** 这是正常状态。先固定导出、量化、转换、推理的接口，再接厂商 SDK。

## 性能分析

- **Q: 计时结果抖动大？** 用 `common/bench.h` 的 warmup + 多次取中位数；GPU 用 cudaEvent 而非主机墙钟。

## 维护提示

- 新增示例时，优先补 README、FAQ 和一个最小可运行命令，不要只加代码不加说明。
- 如果示例依赖真实硬件，至少保留模拟输入或占位脚手架，保证无硬件环境也能理解流程。
