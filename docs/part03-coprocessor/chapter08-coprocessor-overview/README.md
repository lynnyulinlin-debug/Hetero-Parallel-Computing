# 第8章 Device 执行模型与加速器选择

> 目录页：先读 `intro.md`，再按顺序阅读正文小节。
> 返回 [本篇目录](../README.md)。

## 前置条件

- 已理解第7章的 Host 数据所有权、提交节奏与同步边界。

## 导读

- [intro.md](./intro.md)

## 正文小节

- [8.1 主-从内存模型：Host ↔ Device](./01-host-device-memory-model.md)
- [8.2 协处理器的分类](./02-accelerator-types.md)
- [8.3-8.4 性能指标与计算密度判断](./03-metrics-and-roofline.md)

## 最小练习

- 对照 `ch09_vec_add` 与 `ch09_reduce` 的负载特征，估计数据移动、算术强度和潜在瓶颈。

## 完成产出

- 一条 Device 选择记录：是否卸载、优先的 GPU 或 AI 加速器路径、数据移动代价和部署位置约束。
