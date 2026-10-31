# 第5章 OpenMP：循环级数据并行

> 目录页：先读 `intro.md`，再按顺序阅读正文小节。
> 返回 [本篇目录](../README.md)。

## 前置条件

- 已理解第4章中线程与任务的关系；编译器需要支持 OpenMP。

## 导读

- [intro.md](./intro.md)

## 正文小节

- [5.1 编译指令模型](./01-pragma-model.md)
- [5.2 数据共享与私有化](./02-sharing-and-privatization.md)
- [5.3 调度策略](./03-scheduling-strategies.md)
- [5.4 实战：图像模糊的 OpenMP 并行化](./04-blur-example.md)

## 配套实验

- [ch05_openmp_blur](../../../examples/ch05_openmp_blur/README.md)：图像模糊的 OpenMP 教学实验。

## 完成产出

- 一条循环并行对照记录：变量归属、`schedule` 选择、正确性结果和负载变化的影响。
