# 10.1 时间线分析：Nsight Systems

- 系统级时间线：核函数、内存传输、CPU-GPU 重叠一目了然。
- 先看时间线找"时间花在哪"（传输 vs 计算），再决定优化方向。
- 推荐命令：
```bash
nsys profile -o vec_add_report ./vec_add 33554432
nsys profile -o reduce_report ./reduce 33554432
```
- 你要重点看的是：H2D、kernel、D2H 是否串行，是否存在可重叠空间。
- 图建议：Nsight Systems 时间线截图，标注 H2D / kernel / D2H / CPU 主线程。

