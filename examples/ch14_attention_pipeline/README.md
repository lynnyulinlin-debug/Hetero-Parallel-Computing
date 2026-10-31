# 教学实验：注意力流水线

> 对应章节：第14章、第15章。这个示例用教学版 causal attention 串起 GEMM、fused softmax、KV / V 处理和分块调度，是本教程里最接近 FlashAttention 思路的端到端实验。

## 实验目标

- 先把 `QK^T -> softmax -> PV` 这条链跑通。
- 再观察 causal mask、分块和中间张量消除为什么重要。
- 最后把注意力算子放回流水线与系统协同的语境里。

## 运行

```bash
make run
make USE_CUDA=1 run
./attention_pipeline 512 64
```

参数含义：

- `seq_len`：序列长度，默认 `512`
- `head_dim`：头维度，默认 `64`

## 文件结构

| 文件 | 说明 |
|------|------|
| `attention_serial.c` | 串行 causal attention，黄金参考 |
| `attention_openmp.c` | OpenMP 版本，按 query row 并行 |
| `attention_cuda.cu` | CUDA 教学版：单 block 处理一行 query |
| `main.c` | 对比驱动：计时 + 校验 + 加速比报告 |
| `attention.h` | 公共声明 |
| `Makefile` | 默认 CPU-only，可选启用 CUDA |

## 教学要点

- 这个实验不是完整复刻论文实现，而是把 FlashAttention 风格的核心思想拆成教学闭环。
- 重点看三件事：
  - GEMM 风格的 `QK^T`
  - fused softmax 的数值稳定性与中间结果消除
  - `V` 的加权汇聚如何和分块、队列和 stream 调度结合
- CUDA 版的重点在于把一行 query 的处理放到一个 block 里，观察共享内存、阶段边界和同步点。

## 推荐观察

1. 先跑 CPU 串行和 OpenMP，确认结果一致。
2. 再看 CUDA 教学版，观察 `QK`、softmax、`PV` 放进同一 block 后，中间张量往返如何减少。
3. 用 `nsys` / `ncu` 看时间线，重点关注计算、同步和内存流量。

## 动手实验

1. 把 `seq_len` 从 256 改到 1024，观察缓存和共享内存压力如何变化。
2. 把 `head_dim` 从 64 改到 128，理解 GEMM 和 softmax 在 attention 里的相对成本。
3. 把 causal mask 去掉，对比输出和性能，理解阶段依赖如何改变调度。
