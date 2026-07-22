# 10.1 时间线分析：Nsight Systems

Nsight Systems 的作用，是先把整个程序的执行节奏摆出来：CPU 在做什么、GPU 在做什么、传输和计算有没有重叠。

- 系统级时间线：核函数、内存传输、CPU-GPU 重叠一目了然。
- 先看时间线找"时间花在哪"（传输 vs 计算），再决定优化方向。
- 推荐命令：
```bash
nsys profile -o vec_add_report ./vec_add 33554432
nsys profile -o reduce_report ./reduce 33554432
```
- 你要重点看的是：H2D、kernel、D2H 是否串行，是否存在可重叠空间。
- Nsight Systems 时间线截图可标注 H2D / kernel / D2H / CPU 主线程。

从机制上说，这一节的判断顺序是：

1. 先看传输占比大不大。
2. 再看计算和传输有没有重叠。
3. 最后判断问题是“传输太多”还是“计算太少”。

这一步先确定查找方向；第10.2节再去看单核内部细节。
