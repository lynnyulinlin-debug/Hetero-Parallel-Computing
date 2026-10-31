# 附录 A：按学习任务选择环境

本附录不重复平台安装步骤，只帮助读者按当前任务选择环境。

| 学习目标 | 所需环境 | 入口 |
|---|---|---|
| 第一阶段 Task 0-3：本地基础 | Linux、CMake、C 编译器、OpenMP | [CPU / Host](../environment/cpu.md) |
| CUDA 算子与 profiling | NVIDIA GPU、CUDA Toolkit | [NVIDIA CUDA](../environment/nvidia-cuda.md) |
| Task 5：真实边缘视觉 | Jetson Orin Nano、JetPack、TensorRT | [Jetson Orin Nano](../environment/jetson-orin.md) |
| NPU 部署对照 | RK3588/RKNN、昇腾/CANN 等 | [NPU 平台](../environment/npu.md) |
| 复现 CPU/CUDA 构建 | Docker | [容器](../environment/containers.md) |

所有路径先完成 CPU CTest；GPU、Jetson 与 NPU 环境再按实际硬件启用。模拟输出不能替代真实设备测量。
