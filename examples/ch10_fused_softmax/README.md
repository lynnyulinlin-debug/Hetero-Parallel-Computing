# 教学实验：算子融合版 Softmax

> 对应章节：第10章、第13章、第14章。这个实验把 row-wise softmax 作为算子融合的入口，重点看“减少中间张量读写”如何改变端到端效率。

## 实验目标

- 先比较物化中间张量的 CPU 基线、融合 CPU 基线、OpenMP 和 CUDA 实现。
- 再理解 softmax 为什么既是基础算子，也是融合优化的典型入口。
- 最后把它接到 attention / 推理流水线里，观察融合前后的差异。

## 运行

```bash
make run
make USE_CUDA=1 run
./fused_softmax 2048 1024
```

参数含义：

- `rows`：输入行数，默认 `2048`
- `cols`：每行长度，默认 `1024`

## 文件结构

| 文件 | 说明 |
|------|------|
| `softmax_serial.c` | 串行 row-wise softmax，黄金参考 |
| `softmax_unfused.c` | 额外写出并读回指数中间张量的 CPU 对照 |
| `softmax_openmp.c` | OpenMP 并行版，按 row 拆分 |
| `softmax_cuda.cu` | CUDA 融合版：载入、max、exp/sum、归一化一体完成 |
| `main.c` | 对比驱动：计时 + 校验 + 加速比报告 |
| `softmax.h` | 公共声明 |
| `Makefile` | 默认 CPU-only，可选启用 CUDA |

## 教学要点

- Softmax 是注意力和分类头里最常见的归一化算子之一。
- 这个实验的重点不是“快一点”，而是理解中间张量消除为什么会省带宽。
- CUDA 版把关键步骤放进一个 kernel，便于对比 fused vs non-fused。

## 推荐观察

1. 先看 `softmax_unfused` 与 `softmax_serial`，确认中间张量物化和直接融合结果一致。
2. 再看 OpenMP 和 CUDA 融合版，比较端到端耗时和加速比。
3. 用 `nsys` / `ncu` 观察这个看似简单的算子，到底是被带宽、同步还是 launch 开销拖住。

## 动手实验

1. 把 `cols` 从 1024 改到 512 / 2048，观察融合版性能变化。
2. 对照 `softmax_unfused.c`，计算它额外增加的中间张量写入与读取，理解融合真正省掉了什么。
3. 把它作为 attention 的子算子嵌入更大的流水线，再观察其位置和成本。
