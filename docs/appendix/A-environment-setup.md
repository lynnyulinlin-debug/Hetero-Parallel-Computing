# 附录A 实验环境搭建指南

> 目标：让你按需搭好运行各章示例的环境。CPU 示例零门槛，GPU/NPU 按硬件可选。

## A.1 最低环境（运行 CPU 示例）

- Linux（Ubuntu 20.04+ 推荐）
- GCC 9+（自带 OpenMP，`-fopenmp` 即可）
- `make`

验证：

```bash
cd examples/ch09_vec_add && make run
```

看到串行 vs OpenMP 的计时与 `[verify] PASS` 即成功。

## A.2 GPU 环境（NVIDIA）

- NVIDIA GPU（计算能力 5.0+）
- CUDA Toolkit 11+（含 `nvcc`）
- 驱动版本与 CUDA 版本匹配（用 `nvidia-smi` 检查）

验证：

```bash
cd examples/ch09_vec_add && make USE_CUDA=1 run
```

性能分析工具：Nsight Systems（`nsys`）、Nsight Compute（`ncu`）。
如果机器没有 CUDA 设备，`examples/ch09_reduce` 仍会完成 CPU 部分并自动跳过 CUDA 测试。

## A.3 NPU 开发板（可选）

强依赖具体硬件，三选一：

- **RKNN（瑞芯微）**：rknn-toolkit2 + 对应开发板（RK3588 等）。
- **TFLite delegate**：支持 NNAPI/GPU delegate 的设备。
- **昇腾（Ascend）**：CANN 工具链 + Atlas 开发板。

详见 `examples/ch11_npu_deploy/README.md` 的硬件依赖说明。

## A.4 容器环境（推荐用于复现）

见 `docker/` 目录。容器封装编译器、CUDA 运行时与依赖，避免环境漂移。
