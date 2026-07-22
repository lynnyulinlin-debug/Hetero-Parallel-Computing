# 第9章 GPU编程模型（以CUDA为例）

> 本章掌握 CUDA 的线程层次、内存层次与核函数编写，能独立写出并优化 GPU 程序。这是全书的技术核心。

本章分成五个部分：

1. [9.1-9.2 线程层次与内存层次](09-1-thread-and-memory-hierarchy.md)
2. [9.3 核函数编写与启动](09-2-kernel-launch.md)
3. [9.4-9.6 三个实战案例](09-3-three-benchmarks.md)
4. [9.7 流与异步执行](09-4-stream-and-async.md)
5. [9.8-进阶 常见陷阱与统一内存](09-5-pitfalls-and-advanced.md)

## 先看什么

- 先看 [9.1-9.2](09-1-thread-and-memory-hierarchy.md)，再看 [9.3](09-2-kernel-launch.md)，然后是 [9.4-9.6](09-3-three-benchmarks.md)、[9.7](09-4-stream-and-async.md) 和 [9.8-进阶](09-5-pitfalls-and-advanced.md)。

## 本章要点

1. CUDA 三级线程层次 + 多级内存层次是性能优化的基础。
2. 向量加法（带宽受限）vs 矩阵乘（计算受限）揭示加速器的适用边界。
3. 共享内存与流是两大优化杠杆：复用数据、重叠传输。
4. ⚡️ 警惕分支发散、bank 冲突、未检查的内存错误。
