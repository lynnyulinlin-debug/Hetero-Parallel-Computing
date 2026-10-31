# 教学实验：Host 侧调度与异步重叠

> 对应章节：第7章。这个实验把“Host 负责什么”做成可运行的分块流水线，重点看 pinned memory、异步拷贝和 stream overlap 如何把 CPU 组织能力变成系统收益。

## 实验目标

- 先建立一个最小的 Host 调度器：分块、派发、汇合。
- 再在 CUDA 版本里把 H2D / kernel / D2H 放到多条 stream 上。
- 最后理解 Host 的核心价值不是“自己算”，而是“让设备更有效地算”。

## 运行

```bash
make run
make USE_CUDA=1 run
./host_overlap 16777216 1048576 4
```

参数含义：

- `n`：元素个数，默认 `1<<24`
- `chunk`：每个分块的元素数，默认 `1<<20`
- `streams`：CUDA stream 数，默认 `4`

## 文件结构

| 文件 | 说明 |
|------|------|
| `pipeline_serial.c` | Host 侧串行参考实现 |
| `pipeline_cuda.cu` | CUDA 版本：pinned memory + async copy + stream overlap |
| `main.c` | 对比驱动：计时 + 校验 + 加速比报告 |
| `pipeline.h` | 公共声明 |
| `Makefile` | 默认 CPU-only，可选启用 CUDA |

## 教学要点

- 这个实验重点不是 kernel 有多复杂，而是 Host 怎么把数据分块、排队和汇合。
- pinned memory 的价值在于让异步拷贝真正有机会重叠。
- stream overlap 的前提是任务已经被切成相互独立的小块。
- 这个实验最适合和第7章正文一起读，也适合和第14、15章的流水线案例衔接。

## 推荐观察

1. 固定 `n`，改变 `chunk`，观察分块太小或太大时的变化。
2. 固定 `chunk`，改变 `streams`，观察重叠收益和调度开销的平衡。
3. 对比 CPU 参考与 CUDA 版本，理解 Host 的排程作用和设备执行作用各自在哪。

## 动手实验

1. 把 `streams` 改成 1 / 2 / 4 / 8，比较重叠程度。
2. 把 kernel 里的变换从 `tanhf` 改成更轻量的线性映射，观察传输是否更占主导。
3. 把 `chunk` 改成更小的值，理解“过碎的任务”为什么会让调度开销上升。
