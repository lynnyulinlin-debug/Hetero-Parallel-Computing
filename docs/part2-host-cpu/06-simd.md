# 第6章 SIMD向量化

> 这一章解决什么问题：理解单核内部的并行——一条指令同时处理多个数据，这是 CPU 性能常被忽视的一大来源。

本章拆成五个部分，建议按顺序阅读：

1. [6.1 SIMD 概念](06-1-simd-concept.md)
2. [6.2 ARM NEON 与 x86 AVX 简介](06-2-neon-avx.md)
3. [6.3 编译器自动向量化与 `#pragma omp simd`](06-3-auto-vectorization.md)
4. [6.4 intrinsics 基本用法](06-4-intrinsics.md)
5. [6.5 小结：SIMD 在 CPU 体系里的位置](06-5-summary.md)

## 先看什么

- 如果你想先理解 SIMD 是什么，先看 [6.1](06-1-simd-concept.md)。
- 如果你想知道 NEON / AVX 差在哪，先看 [6.2](06-2-neon-avx.md)。
- 如果你想优先让编译器帮你向量化，先看 [6.3](06-3-auto-vectorization.md)。
- 如果你需要手写底层向量代码，先看 [6.4](06-4-intrinsics.md)。
- 如果你想知道 SIMD 在整个 CPU 体系里的位置，直接看 [6.5](06-5-summary.md)。

## 本章要点

1. SIMD 是核内数据并行，与多线程可叠加。
2. 优先依赖编译器自动向量化，并用报告验证效果。
3. intrinsics 是最后手段，可控但可移植性差。
4. SIMD 的真正价值，是让 CPU 在不增加线程的情况下继续提升单核吞吐。

