# 第10章 GPU 性能分析与云端视角

> 目录页：先读 `intro.md`，再按顺序阅读正文小节。
> 返回 [本篇目录](../README.md)。

## 前置条件

- 已完成第9章的 kernel 映射；运行 `nsys` / `ncu` 需要 CUDA 工具链和 GPU，缺少硬件时先阅读既有报告与指标解释。

## 导读

- [intro.md](./intro.md)

## 正文小节

- [10.1 时间线分析：Nsight Systems](./01-timeline-analysis.md)
- [10.2 内核指令级分析：Nsight Compute（入门级解读）](./02-ncu-analysis.md)
- [10.3-10.5 指标解读与实战对照](./03-metrics-and-practice.md)

## 配套实验

- [GEMM `ch09_gemm`](../../../examples/ch09_gemm/README.md)
- [Fused Softmax `ch10_fused_softmax`](../../../examples/ch10_fused_softmax/README.md)
- [向量加法 `ch09_vec_add`](../../../examples/ch09_vec_add/README.md)
- [归约 `ch09_reduce`](../../../examples/ch09_reduce/README.md)

## 完成产出

- 一条测量记录：时间线证据、一个局部指标、瓶颈假设、代码改动，以及复测后的结论。
