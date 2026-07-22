# 第6章 SIMD向量化

> 本章说明单核内部的并行：一条指令同时处理多个数据，这是 CPU 性能常被忽视的一大来源。

SIMD 的机制核心，是把一个标量操作扩展成“向量 lane 上的并行执行”。读这章时要始终记住：它不是多线程的替代品，而是单个线程内部的并行能力，常常和多线程叠加使用。

本章分成五个部分：

1. [6.1 SIMD 概念](06-1-simd-concept.md)
2. [6.2 ARM NEON 与 x86 AVX 简介](06-2-neon-avx.md)
3. [6.3 编译器自动向量化与 `#pragma omp simd`](06-3-auto-vectorization.md)
4. [6.4 intrinsics 基本用法](06-4-intrinsics.md)
5. [6.5 小结：SIMD 在 CPU 体系里的位置](06-5-summary.md)

## 先看什么

- 先看 [6.1](06-1-simd-concept.md)，再看 [6.2](06-2-neon-avx.md)，然后是 [6.3](06-3-auto-vectorization.md)、[6.4](06-4-intrinsics.md) 和 [6.5](06-5-summary.md)。

## 本章要点

1. SIMD 是核内数据并行，与多线程可叠加。
2. 优先依赖编译器自动向量化，并用报告验证效果。
3. intrinsics 是最后手段，可控但可移植性差。
4. SIMD 的真正价值，是让 CPU 在不增加线程的情况下继续提升单核吞吐。
5. 机制上，SIMD 依赖向量宽度、数据对齐、lane 复用和编译器识别模式。
