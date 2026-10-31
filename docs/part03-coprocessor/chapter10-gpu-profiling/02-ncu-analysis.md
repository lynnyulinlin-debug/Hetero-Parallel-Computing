# 10.2 内核指令级分析：Nsight Compute（入门级解读）

这一节从“程序整体怎么跑”切到“单个核函数为什么慢”，把第10章的诊断视角从系统级收窄到核级。

Nsight Compute 的作用，是把一个核函数内部为什么慢讲清楚：是算得不够满、访存太重、还是同步/原子太多。

- 单个核函数的指令、内存、占用率细节。
- 入门只需看几个关键指标，不必读懂全部。
- 推荐命令：
```bash
ncu --set full ./reduce 33554432
ncu --metrics sm__warps_active.avg.pct_of_peak_sustained_active,dram__throughput.avg.pct_of_peak_sustained_elapsed ./reduce 33554432
```
- 对 `reduce` 这类例子，要重点观察 shared memory、warp divergence、atomic 相关指标。

从机制上说，Nsight Compute 更像一张“核函数体检单”：

1. 占用率告诉你并行度有没有铺开。
2. 带宽指标告诉你是不是被访存卡住。
3. bank 冲突和原子压力告诉你内部争用是不是太高。

它的价值不是让你记住更多指标，而是让你把“核函数慢”拆成几类可解释的原因。这样回到代码时，修改方向才不会乱。

**小结**
Nsight Compute 的任务，是把“慢”拆成可操作的原因：并行度、带宽、冲突和原子争用。只有先知道慢在哪个层次，优化才不会凭感觉乱改。
