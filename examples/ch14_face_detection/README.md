# 教学实验：实时视频流人脸检测

> 对应章节：第14章、第15章。这个实验把视频采集、推理和后处理拆成三级流水，用来观察阶段边界、队列调度和后端切换如何影响端到端表现。

## 实验目标

- 先把 CPU 采集/解码、GPU/NPU 推理和 CPU 后处理分成明确阶段。
- 再看后端切换如何影响吞吐、延迟和队列长度。
- 最后把这条流水线作为第15章任务图与协同调度的案例。

## 运行入口

```bash
python3 pipeline.py --backend cpu
python3 pipeline.py --backend gpu
python3 pipeline.py --backend npu
python3 pipeline.py --backend gpu --source list --input-list models/source.list
```

## 目录结构

| 路径 | 说明 |
|---|---|
| `pipeline.py` | 主流水线：采集 -> 推理 -> 后处理 |
| `stages/capture_decode.py` | CPU 采集与解码 |
| `stages/infer_cpu.py` | CPU 基线后端 |
| `stages/infer_gpu.py` | GPU 后端接口 |
| `stages/infer_npu.py` | NPU 后端接口 |
| `stages/postprocess.py` | CPU 后处理 |
| `models/` | 输入源和模型说明 |

## 教学要点

- 这个实验的重点不是某个单独算子，而是整条流水线如何协同。
- 队列、后端切换和阶段耗时，是最值得关注的三个观察点。
- `gpu` / `npu` 后端先保留统一接口，真实 SDK 接入时只替换对应 `infer_*` 模块。
- 第一个真实硬件目标是 [Jetson Orin Nano](platforms/jetson-orin-nano/README.md)：先用图片预检模型，再以[固定视频](platforms/jetson-orin-nano/video-baseline.md)产出首份实测报告；[RK3588](platforms/rk3588/README.md) 保留为后续 NPU 对照。

## 推荐观察

1. 先用 `--backend cpu` 跑通模拟帧流，确认阶段和队列都能工作。
2. 再切换 `--backend gpu` 或 `--backend npu`，观察端到端帧率和队列长度变化。
3. 对照不同输入源，理解摄像头、文件列表和流媒体在调度上有什么差异。

## 动手实验

1. 把推理后端从 `cpu` 切到 `gpu` 或 `npu`，记录帧率变化。
2. 调整队列大小，观察背压和吞吐如何变化。
3. 把 `stages/infer_*` 里的模型替换成自己的后端实现，记录兼容问题和耗时分布。

## 配套阅读

- [FlashAttention 风格注意力流水线 `ch14_attention_pipeline`](../ch14_attention_pipeline/README.md)：先看注意力流水线的教学版，再回到这里看多阶段协同。
- [第14章导读](../../docs/part04-systems/chapter14-end-to-end-cases/intro.md)：先看案例与实验如何对应，再回到本项目看流水线结构。
