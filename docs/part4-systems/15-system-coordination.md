# 第15章 异构计算中的系统协同

> 本章把单设备优化推进到多设备协同：如何在 CPU、多 GPU、NPU 之间划分工作并保证正确性。

本章分成五个部分：

1. [15.1 CPU 与协处理器的工作划分原则](15-1-work-splitting.md)
2. [15.2 任务图调度：依赖关系与异步执行](15-2-dag-scheduling.md)
3. [15.3 多协处理器协同](15-3-multi-accelerator-coordination.md)
4. [15.4 缓存一致性与内存模型](15-4-memory-model.md)
5. [15.5-15.6 回到案例与实验对照](15-5-case-and-experiments.md)

## 先看什么

- 先看 [15.1](15-1-work-splitting.md)，再看 [15.2](15-2-dag-scheduling.md)，然后是 [15.3](15-3-multi-accelerator-coordination.md)、[15.4](15-4-memory-model.md) 和 [15.5-15.6](15-5-case-and-experiments.md)。

## 本章要点

1. 工作划分按算术强度、数据量、延迟敏感度决定。
2. DAG + 异步执行是多设备协同的调度基础。
3. 跨设备共享数据时要显式同步。
4. 队列和同步点的设计比单点优化更重要。
5. 第14章案例1提供了本章的最小系统样例。
