# 第5章 OpenMP：高层并行抽象

> 本章说明如何用编译指令（pragma）以最小改动把串行循环并行化，掌握 CPU 数据并行最常用的工具。

OpenMP 的机制核心，是把“并行意图”写进编译指令，再由编译器和运行时把循环拆给多个线程。它不是替你消灭并行复杂度，而是把复杂度从“手写线程控制”转成“声明共享、私有化和调度策略”。

本章分成四个部分：

1. [5.1 编译指令模型](05-1-pragma-model.md)
2. [5.2 数据共享与私有化](05-2-sharing-and-privatization.md)
3. [5.3 调度策略](05-3-scheduling-strategies.md)
4. [5.4 实战：图像模糊的 OpenMP 并行化](05-4-blur-example.md)

## 先看什么

- 先看 [5.1](05-1-pragma-model.md)，再看 [5.2](05-2-sharing-and-privatization.md)，然后是 [5.3](05-3-scheduling-strategies.md) 和 [5.4](05-4-blur-example.md)。

## 本章要点

1. OpenMP 用 pragma 实现低侵入式并行，适合循环密集型代码。
2. 数据共享子句决定正确性，归约子句安全合并部分结果。
3. 调度策略需按负载均衡程度选择。
4. OpenMP 的价值不是“完全替你写并行”，而是让 CPU 端并行化成本足够低。
5. 机制上，OpenMP 的关键在于“编译期标注 + 运行时调度 + 明确的数据作用域”三件事一起工作。
