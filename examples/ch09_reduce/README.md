# 实战2：归约（Reduce）

> 对应章节：第9.5节。演示**共享内存优化前后的性能差异**，是理解 GPU 内存层次的关键示例。

## 状态

✅ 已补齐最小可运行骨架。当前实现包含串行、OpenMP、CUDA 朴素版、CUDA 共享内存版，便于直接对比结果与性能。
✅ 已验证 `USE_CUDA=1` 可成功编译；在当前环境运行时若无 CUDA 设备，会自动跳过 CUDA 测试并保留 CPU 结果。

| 文件 | 说明 |
|------|------|
| `reduce_serial.c` | 串行求和，黄金参考 |
| `reduce_openmp.c` | OpenMP `reduction(+:sum)` 子句 |
| `reduce_cuda_naive.cu` | 朴素 GPU 归约（全局内存 + 树形归约，存在分支发散） |
| `reduce_cuda_shared.cu` | 共享内存优化版（消除 bank 冲突、warp 级展开） |
| `main.c` | 对比驱动，复用 `../common/` 工具 |
| `reduce.h` | 公共声明，统一函数签名 |
| `Makefile` | 默认 CPU-only，可选启用 CUDA |

## 教学要点

- 归约存在跨线程数据依赖，不像向量加法那样"尴尬并行"，需要同步与多级归约。
- 朴素版 → 共享内存版的演进，对应第9.5节和第10章 profiler 定位瓶颈。
- 浮点累加顺序不同会带来微小误差，`verify.h` 用相对容差而非精确相等。

## 运行方式

```bash
make run            # 仅 CPU（串行 + OpenMP）
make USE_CUDA=1 run # 含 CUDA 实现（需 NVIDIA GPU + nvcc）
./reduce 33554432   # 自定义元素个数
```

## 动手实验

1. 对比朴素版与共享内存版的加速比，用 Nsight Compute 看共享内存吞吐与 bank 冲突。
2. 尝试 warp shuffle（`__shfl_down_sync`）做最后一级归约，对比性能。
3. 先用 `nsys` 看整体时间线，再用 `ncu` 看单核函数细节，建立从系统到内核的分析习惯。
