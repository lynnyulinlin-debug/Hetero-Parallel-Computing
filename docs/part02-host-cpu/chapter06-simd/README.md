# 第6章 SIMD：指令级数据并行

> 目录页：先读 `intro.md`，再按顺序阅读正文小节。
> 返回 [本篇目录](../README.md)。

## 前置条件

- 已理解第5章的规则循环和数据作用域；需要目标平台对应的编译器向量化报告。

## 导读

- [intro.md](./intro.md)

## 正文小节

- [6.1 SIMD 概念](./01-simd-concept.md)
- [6.2 ARM NEON 与 x86 AVX 简介](./02-neon-avx.md)
- [6.3 编译器自动向量化与 `#pragma omp simd`](./03-auto-vectorization.md)
- [6.4 intrinsics 基本用法（🔬 进阶，点到为止）](./04-intrinsics.md)
- [6.5 验证向量化与停止优化](./05-verification-and-stop-criteria.md)

## 配套实验

- [ch06_simd_layernorm](../../../examples/ch06_simd_layernorm/README.md)：LayerNorm 的 SIMD 教学实验。

## 完成产出

- 一条向量化证据记录：标量/向量版本的正确性、编译器报告、数据布局条件和停止优化理由。
