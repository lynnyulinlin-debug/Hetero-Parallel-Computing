# 第8章 异构协处理器概述

> 这一章解决什么问题：在写任何 GPU/NPU 代码之前，先建立"主-从内存模型"和"该不该上加速器"的判断框架。

本章拆成三个部分，建议按顺序阅读：

1. [8.1 主-从内存模型：Host ↔ Device](08-1-host-device-memory-model.md)
2. [8.2 协处理器的分类](08-2-accelerator-types.md)
3. [8.3-8.4 性能指标与计算密度判断](08-3-metrics-and-roofline.md)

## 先看什么

- 如果你最关心 CPU 和加速器为什么要分开，先看 [8.1](08-1-host-device-memory-model.md)。
- 如果你想知道 GPU 和 NPU 的差别，先看 [8.2](08-2-accelerator-types.md)。
- 如果你想判断该不该上加速器，直接看 [8.3-8.4](08-3-metrics-and-roofline.md)。

## 本章要点

1. Host/Device 各自独立内存，数据搬运是核心复杂性。
2. GPU 通用、NPU 专用，设计哲学不同。
3. 📋 用算术强度判断是否值得上加速器，别被"峰值算力"误导。

