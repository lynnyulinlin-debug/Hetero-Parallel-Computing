# 第9章 GPU 编程与基础算子（以 CUDA 为例）

> 目录页：先读 `intro.md`，再按顺序阅读正文小节。
> 返回 [本篇目录](../README.md)。

## 前置条件

- 已完成第8章的 Device 判断；运行 CUDA 示例需要 CUDA 工具链和可用 NVIDIA GPU，无硬件时可完成线程与内存映射推导。

## 导读

- [intro.md](./intro.md)

## 正文小节

- [9.1-9.2 CUDA 线程层次与内存层次](./01-thread-and-memory-hierarchy.md)
- [9.3 核函数编写与启动（`<<<>>>` 语法）](./02-kernel-launch.md)
- [9.4-9.6 三个实战案例](./03-three-benchmarks.md)
- [9.7 流（Stream）与异步执行](./04-stream-and-async.md)
- [9.8 ⚡️ 常见陷阱](./05-pitfalls-and-advanced.md)

## 配套实验

- [向量加法 `ch09_vec_add`](../../../examples/ch09_vec_add/README.md)
- [归约 `ch09_reduce`](../../../examples/ch09_reduce/README.md)
- [矩阵乘 `ch09_gemm`](../../../examples/ch09_gemm/README.md)

## 完成产出

- 一条 kernel 映射记录：线程如何覆盖数据、数据放在哪级内存、同步点在哪里，以及当前瓶颈假设是什么。
