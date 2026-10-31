# 第7章 Host-Device 协同：数据、异步与同步

> 目录页：先读 `intro.md`，再按顺序阅读正文小节。
> 返回 [本篇目录](../README.md)。

## 前置条件

- 已理解第4-6章的任务、循环和 SIMD 层次；无 CUDA 环境时可只运行 Host 参考路径。

## 导读与正文

1. [导读：Host 作为异构系统控制面](./intro.md)
2. [7.1 数据放置与页锁定内存](./01-data-placement-and-pinned-memory.md)
3. [7.2 异步提交与同步收口](./02-async-submission-and-synchronization.md)
4. [7.3 数据搬运与流重叠](./03-transfer-and-stream-overlap.md)

## 配套实验

- [ch07_host_overlap](../../../examples/ch07_host_overlap/README.md)：Host 调度、pinned memory 和 stream overlap 的最小实验。

## 完成产出

- 一张 Host 控制面记录：数据所有权、提交顺序、同步点、缓冲区复用条件，以及可观察到的重叠或空转。
