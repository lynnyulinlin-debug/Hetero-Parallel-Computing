# 第5章 OpenMP：高层并行抽象

> 这一章解决什么问题：用编译指令（pragma）以最小改动把串行循环并行化，掌握 CPU 数据并行最常用的工具。

本章拆成四个部分，建议按顺序阅读：

1. [5.1 编译指令模型](05-1-pragma-model.md)
2. [5.2 数据共享与私有化](05-2-sharing-and-privatization.md)
3. [5.3 调度策略](05-3-scheduling-strategies.md)
4. [5.4 实战：图像模糊的 OpenMP 并行化](05-4-blur-example.md)

## 先看什么

- 如果你想先理解 OpenMP 怎么“少改代码就并行”，先看 [5.1](05-1-pragma-model.md)。
- 如果你最担心正确性问题，先看 [5.2](05-2-sharing-and-privatization.md)。
- 如果你想知道 static / dynamic / guided 怎么选，先看 [5.3](05-3-scheduling-strategies.md)。
- 如果你想看一个更接近工程的例子，直接看 [5.4](05-4-blur-example.md)。

## 本章要点

1. OpenMP 用 pragma 实现低侵入式并行，适合循环密集型代码。
2. 数据共享子句决定正确性，归约子句安全合并部分结果。
3. 调度策略需按负载均衡程度选择。
4. OpenMP 的价值不是“完全替你写并行”，而是让 CPU 端并行化成本足够低。

