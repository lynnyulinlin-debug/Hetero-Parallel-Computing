# 10.2 内核指令级分析：Nsight Compute（入门级解读）

- 单个核函数的指令、内存、占用率细节。
- 入门只需看几个关键指标，不必读懂全部。
- 推荐命令：
```bash
ncu --set full ./reduce 33554432
ncu --metrics sm__warps_active.avg.pct_of_peak_sustained_active,dram__throughput.avg.pct_of_peak_sustained_elapsed ./reduce 33554432
```
- 对 `reduce` 这类例子，要重点观察 shared memory、warp divergence、atomic 相关指标。

