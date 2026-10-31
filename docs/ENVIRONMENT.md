# 环境选择与验证

> 本页只负责选择环境与进入验证路径。安装命令、平台前置条件和排障分别放在 `docs/environment/`；最短学习路径见 [USAGE](USAGE.md)。

## 先选目标

| 目标 | 最低环境 | 验证入口 | 详细说明 |
|---|---|---|---|
| 第一阶段本地基础（Task 0-3） | Linux、C 编译器、CMake、OpenMP | `ctest -L cpu` | [CPU / Host](environment/cpu.md) |
| CUDA 算子 | NVIDIA GPU、驱动、CUDA Toolkit | `ctest -L cuda` | [NVIDIA CUDA](environment/nvidia-cuda.md) |
| Jetson 边缘视觉 | Jetson Orin Nano、JetPack、TensorRT | `ch14_face_detection --mode real` | [Jetson Orin Nano](environment/jetson-orin.md) |
| NPU 对照部署 | RK3588/RKNN、昇腾/CANN 等 | 第11章部署报告 | [NPU 平台](environment/npu.md) |
| 可复现构建 | Docker；CUDA 路径另需 NVIDIA Container Toolkit | 容器内 CTest | [容器](environment/containers.md) |

## 通用验证顺序

1. 先完成 CPU 构建与 `ctest -L cpu`。
2. 有 NVIDIA GPU 时，再启用 CUDA target 与 `ctest -L cuda`。
3. 有 Jetson 时，使用真实视频与 TensorRT 适配器导出设备报告。
4. 最后按需要接入 RK3588 或其他 NPU，作为部署约束对照。

模拟项目必须输出 `execution_mode: simulated`；真实设备报告必须包含平台、运行时版本、模型、输入和测量口径。具体要求见 [代码与测试框架](ENGINEERING.md)。

## 没有硬件时能完成什么

没有 NVIDIA GPU、Jetson 或 NPU 开发板时，仍可完成第一阶段：Task 0-3 走 CPU 路径，Task 4-6 走模拟或设计路径。不能得出的结论是 GPU/NPU 的真实性能、功耗、驱动兼容性和厂商 SDK 行为。

| 可执行 | 命令或项目 | 结论边界 |
|---|---|---|
| CPU 正确性 | `ctest -L cpu` | 验证实现与参考结果一致，不代表 GPU 性能 |
| CUDA 构建 | 有 Toolkit 时 `ctest -L cuda` | 无设备时测试跳过，不代表 kernel 已在 GPU 验证 |
| 部署模拟 | `ctest -L python` | 验证报告 schema 与失败语义，不代表板端推理 |
| 模拟流水线 | `ch14_face_detection --mode simulated` | 验证队列、阶段与指标结构，不代表端到端设备吞吐 |

## 相关文档

- [CPU / Host 环境](environment/cpu.md)
- [NVIDIA CUDA 环境](environment/nvidia-cuda.md)
- [Jetson Orin Nano 环境](environment/jetson-orin.md)
- [NPU 平台环境](environment/npu.md)
- [容器环境](environment/containers.md)
- [环境排障](environment/troubleshooting.md)
