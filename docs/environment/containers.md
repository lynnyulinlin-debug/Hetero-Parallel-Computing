# 容器环境

容器用于复现 CPU 构建、Python 报告契约和 CUDA target 编译，不用于替代真实设备运行时。

| 目标 | 镜像 | 通过条件 |
|---|---|---|
| 第一阶段 CPU / 模拟路径 | `heteropar:cpu` | `ctest -L 'cpu|python'` 通过 |
| NVIDIA CUDA 编译 | `heteropar:cuda` | CUDA target 构建成功；有 GPU 时再运行 CUDA CTest |

```bash
# CPU：构建阶段和默认运行阶段都会执行 cpu|python CTest
docker build -f docker/Dockerfile.cpu -t heteropar:cpu .
docker run --rm heteropar:cpu

# CUDA：宿主机需有 NVIDIA 驱动和 NVIDIA Container Toolkit
docker build -f docker/Dockerfile.cuda -t heteropar:cuda .
docker run --rm --gpus all heteropar:cuda
```

CUDA 镜像在构建时只编译 CUDA target，并执行 CPU/Python CTest；GPU kernel 正确性必须在运行容器时由 `--gpus all` 提供设备。没有设备时，CUDA 标签测试会跳过，不可写为硬件验收通过。

Jetson、RKNN、CANN、模型权重和真实视频输入不进入通用镜像。它们分别按 [Jetson 环境](jetson-orin.md)、[边缘 NPU 环境](npu.md) 和 [昇腾/CANN 专题](../phase2/ascend-cann.md) 在目标设备上验证。

镜像内容、排除规则与发布前检查见 [docker/README.md](../../docker/README.md)。
