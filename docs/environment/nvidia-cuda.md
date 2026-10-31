# NVIDIA CUDA 环境

适用于第9-10章 CUDA 算子与 Jetson 之前的本地 GPU 验证。

## 前置条件

- 可用 NVIDIA GPU 与匹配驱动
- CUDA Toolkit，`nvcc` 可在 `PATH` 中找到
- 可选：Nsight Systems (`nsys`) 与 Nsight Compute (`ncu`)

## 构建与测试

```bash
cmake -S . -B build-cuda -DHETEROPAR_ENABLE_CUDA=ON
cmake --build build-cuda
ctest --test-dir build-cuda -L cuda --output-on-failure
```

没有可用 GPU 时，CUDA 硬件测试会跳过；CPU 测试仍必须通过。可单独运行 `ch09_vec_add_cuda`、`ch09_reduce_cuda`、`ch09_gemm_cuda` 对照 CPU 参考实现。

## 下一步

边缘 NVIDIA 设备请转到 [Jetson Orin Nano 环境](jetson-orin.md)，不要把桌面 CUDA 结果当作 Jetson 实测结果。
