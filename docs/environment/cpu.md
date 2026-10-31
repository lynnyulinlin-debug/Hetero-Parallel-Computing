# CPU / Host 环境

适用于第一阶段 Task 0-3 的全部 CPU 示例与仓库级回归测试。

## 前置条件

- Linux
- CMake 3.18+
- GCC 9+ 或 Clang，且支持 OpenMP
- `make` 或 Ninja

## 验证

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build -L cpu --output-on-failure
```

通过标准：CPU 标签下的测试全部通过。性能数字只用于本机对照，不作为通过条件。

## 下一步

- 看 [NVIDIA CUDA 环境](nvidia-cuda.md) 启用 GPU 算子。
- 看 [第一阶段任务](../TASKS.md) 建立本地异构计算基础。
