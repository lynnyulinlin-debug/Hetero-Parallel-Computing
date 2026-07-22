# 第10章 GPU性能分析工具链

> 本章把优化方式从“猜瓶颈”转向“测瓶颈”，并用 NVIDIA profiler 完成这条路径。

第10章的机制核心，不是记住两个工具的名字，而是建立一条稳定的诊断路径：先看系统时间线，再看单核指标，最后回到代码修改和复测。只要这条路径清楚，后面的优化就不会靠猜。

本章分成三个部分：

1. [10.1 时间线分析：Nsight Systems](10-1-timeline-analysis.md)
2. [10.2 内核指令级分析：Nsight Compute](10-2-ncu-analysis.md)
3. [10.3-10.5 指标解读与实战对照](10-3-metrics-and-practice.md)

## 先看什么

- 先看 [10.1](10-1-timeline-analysis.md)，再看 [10.2](10-2-ncu-analysis.md)，最后看 [10.3-10.5](10-3-metrics-and-practice.md)。

## 本章要点

1. 优化前先用时间线工具定位时间花在哪。
2. 占用率、内存吞吐、bank 冲突是最常用的三个诊断指标。
3. profiler 驱动的"测—改—验证"闭环是可靠优化的唯一正道。
4. Nsight Systems 负责看系统级节奏，Nsight Compute 负责看单核函数内部细节。
