# 第3章 并行模式入门

> 目录页：先读 `intro.md`，再按顺序阅读正文小节。
> 返回 [本篇目录](../README.md)。

## 前置条件

- 已理解第1章的性能约束和第2章的共享状态、同步边界。

## 导读

- [intro.md](./intro.md)

## 正文小节

- [3.1 基本并行模式](./01-basic-parallel-modes.md)
- [3.2 任务并行 vs 数据并行](./02-task-vs-data-parallel.md)
- [3.3 吞吐量 vs 延迟](./03-throughput-vs-latency.md)
- [3.4 怎么把模式翻译成硬件选择](./04-hardware-mapping.md)
- [3.5 模式选择记录卡](./05-chapter-role.md)

## 最小练习

- 用 [3.5 模式选择记录卡](./05-chapter-role.md) 描述一个熟悉的计算或服务流程，不根据设备名称直接下结论。

## 完成产出

- 一页模式选择记录：工作单元、依赖、吞吐/延迟目标、候选模式和硬件约束。
