# Task 4 参考记录：受限算子优化

> 本记录以 row-wise softmax 说明“消除中间张量”如何形成可验证对照。它是 CPU-only 观察，不等同于 CUDA kernel profile 或端到端服务测量。

## 对照设计

| 实现 | 中间数据路径 | 验证目的 |
|---|---|---|
| `softmax_unfused` | `exp(x - max)` 写入独立 scratch buffer，再读回并写最终输出 | 表示额外 materialized tensor 的读写成本 |
| `softmax_serial` | 临时指数直接写入最终输出，随后原地归一化 | 消除独立中间张量的 CPU 融合基线 |
| `softmax_openmp` | 与融合 CPU 路径相同，按 row 并行 | 分离线程调度影响与中间张量影响 |
| `softmax_cuda` | CUDA 融合实现 | 仅在有设备时验证 kernel 与设备侧性能 |

所有实现以相同的稳定 softmax 参考结果校验。`softmax_unfused` 的 scratch 在计时外分配，避免把内存分配误当成算子融合收益。

## 本机观察

```bash
./build/ch10_fused_softmax 256 512
```

在 2026-10-21 的 CPU-only 工作站上，`softmax_unfused` 中位数约 `0.736 ms`，融合 CPU 路径约 `0.722 ms`，两者均通过数值校验。这个约 `1.02x` 差异很小，符合输入可落入缓存、CPU 循环开销占比较高的情况。

因此本次可得出的结论是：额外中间张量确实改变了读写路径，且对照实现结果一致；不能得出的结论是“融合在所有输入和所有硬件上必然显著更快”。

## 指标边界

- 当前记录的是单个 CPU 算子中位数，不是服务吞吐或排队延迟。
- GPU 上还需要分开记录 Host-Device 传输、kernel 时间和回读时间。
- 注意力流水线只有在使用相同序列长度、head dimension、精度和测量口径时，才能与 softmax 对照建立因果关系。

下一步进入 Task 5 时，不把这里的 CPU 时间写入部署报告；部署报告只描述模型、运行时、输入、`execution_mode` 与设备侧指标。
