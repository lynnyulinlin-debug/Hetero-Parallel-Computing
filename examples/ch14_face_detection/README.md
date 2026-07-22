# 端到端案例：实时视频流人脸检测

> 对应章节：第14章案例1。完整异构流水线：CPU 采集+解码 → GPU/NPU 推理 → CPU 后处理。

## 状态

✅ 已补齐可运行的流水线骨架。当前重点不是接入真实模型，而是先把阶段划分、后端切换和数据流接口固定下来。

```
ch14_face_detection/
├── pipeline.py                  # 主流水线：解码 → 推理 → 后处理 → 渲染
├── stages/
│   ├── capture_decode.py        # CPU：视频采集与解码
│   ├── infer_cpu.py             # CPU 基线推理后端
│   ├── infer_gpu.py             # GPU 推理后端
│   ├── infer_npu.py             # NPU 推理后端（与 ch11 共享转换产物）
│   └── postprocess.py           # CPU：NMS、框绘制
├── models/                      # 模型文件说明
└── README.md
```

## 教学要点

- 系统级协同（第15章）：各阶段在 CPU/GPU/NPU 间的划分原则与流水线重叠。
- 异步与任务图：解码、推理、后处理三级流水，用队列解耦、隐藏延迟。
- 性能数据：对比 GPU 后端 vs NPU 后端的吞吐/延迟/功耗。
- 部署注意事项：环境依赖、版本兼容、稳定性。
- 最小可运行目标：即使没有真实模型，也要能跑通模拟帧流、后端切换和结果输出。
- 观测指标：端到端帧率、队列最大长度、各阶段平均耗时。

## 综合作业

把推理后端从 GPU 切到 NPU（替换 `stages/infer_*`），测量端到端帧率变化，记录算子兼容与精度问题。

## 运行说明

```bash
python3 pipeline.py --backend gpu
python3 pipeline.py --backend npu
python3 pipeline.py --backend cpu
python3 pipeline.py --backend gpu --source list --input-list models/source.list
python3 pipeline.py --backend gpu --source camera
python3 pipeline.py --backend gpu --source stream
```

当前版本的 `gpu` / `npu` 后端先保留统一接口，后续接入真实 SDK 时只替换 `stages/infer_*`。
输出会包含：

- 端到端帧率
- `capture_q_max` / `infer_q_max` 最大队列长度
- capture / infer / post 的平均阶段耗时

## 输入约定

- `--source simulated`：生成模拟帧，适合本地无视频环境
- `--source list`：从文本列表读取来源路径，后续可替换为真实视频文件或摄像头索引
- `--source camera`：单摄像头来源
- `--source stream`：单网络流来源

真实输入样例见 [models/video_source_example.md](models/video_source_example.md)。
摄像头与混合来源样例见 [models/camera_source_example.md](models/camera_source_example.md)。
流媒体来源样例见 [models/stream_source_example.md](models/stream_source_example.md)。
