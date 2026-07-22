# 第10章 GPU性能分析工具链

> 这一章解决什么问题：从"猜瓶颈"转向"测瓶颈"——掌握 NVIDIA profiler，用数据驱动优化。

本章拆成三个部分，建议按顺序阅读：

1. [10.1 时间线分析：Nsight Systems](10-1-timeline-analysis.md)
2. [10.2 内核指令级分析：Nsight Compute](10-2-ncu-analysis.md)
3. [10.3-10.5 指标解读与实战对照](10-3-metrics-and-practice.md)

## 先看什么

- 如果你想先知道时间都花在哪，先看 [10.1](10-1-timeline-analysis.md)。
- 如果你想看单个核函数怎么分析，先看 [10.2](10-2-ncu-analysis.md)。
- 如果你想把工具和例子连起来，直接看 [10.3-10.5](10-3-metrics-and-practice.md)。

## 本章要点

1. 优化前先用时间线工具定位时间花在哪。
2. 占用率、内存吞吐、bank 冲突是最常用的三个诊断指标。
3. profiler 驱动的"测—改—验证"闭环是可靠优化的唯一正道。

