# 第4章 Host 任务并行：线程与线程池

> 目录页：先读 `intro.md`，再按顺序阅读正文小节。
> 返回 [本篇目录](../README.md)。

## 前置条件

- 已理解第2章的任务队列时序；需要可用的 C 编译环境和仓库构建目录。

## 导读

- [intro.md](./intro.md)

## 正文小节

- [4.1 POSIX 线程（pthread）入门](./01-pthread-basics.md)
- [4.2 线程池设计与实现（🔬 进阶）](./02-thread-pool.md)
- [4.3 线程池与 OpenMP 的选择边界](./03-model-selection.md)

## 配套实验

- [ch04_task_pool](../../../examples/ch04_task_pool/README.md)：线程池与任务队列的最小实验。

## 完成产出

- 一条任务生命周期记录：任务如何入队、谁等待、何时在锁外执行、如何关闭并汇合结果。
