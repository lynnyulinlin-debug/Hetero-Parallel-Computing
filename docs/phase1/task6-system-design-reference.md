# Task 6 参考记录：边缘视觉系统设计

> 本记录用教学型人脸检测流水线完成 Task 6 的系统设计。所有运行结果来自 `execution_mode: simulated` 和 `metric_source: teaching_pipeline`，不代表 Jetson、摄像头或真实视频性能。

## 场景与设备分工

场景是单路边缘人脸检测。首轮真实设备目标是 Jetson Orin Nano，但本记录只验证其系统结构：

```text
固定视频 / 摄像头
    -> CPU capture + decode + preprocess
    -> GPU TensorRT inference
    -> CPU postprocess + result handoff
```

| 阶段 | 当前模拟实现 | 真实 Jetson 的预期职责 | 同步边界 |
|---|---|---|---|
| capture | CPU 生产模拟帧 | CPU/硬件解码准备视频帧 | 写入 `capture_q`；队列满时背压 |
| infer | CPU 教学后端 | GPU TensorRT 推理 | 取走输入帧，写入 `infer_q` |
| postprocess | CPU 教学后端 | CPU 解析检测框、NMS、交接结果 | 取走结果；最终停止标记收口 |

首要约束是端到端延迟和内存上界，而不是单个 kernel 的峰值吞吐。队列长度必须有限，避免推理变慢时无界缓存视频帧。

## 可复测改动

```bash
python3 examples/ch14_face_detection/pipeline.py \
  --backend cpu --frames 30 --queue-size 2 --report-json /tmp/task6-face.json
python3 examples/ch14_face_detection/pipeline.py \
  --backend cpu --frames 30 --queue-size 1 --report-json /tmp/task6-face-q1.json
```

本次模拟中：

| 队列容量 | `capture_q_max` | `infer_q_max` | FPS | 解释 |
|---:|---:|---:|---:|---|
| 2 | 2 | 1 | 约 241.76 | 采集可以领先推理两个帧槽位 |
| 1 | 1 | 1 | 约 241.64 | 采集更早受到背压，模拟吞吐基本不变 |

可得出的结论是：队列上限确实改变积压边界。不可得出的结论是：真实视频下队列 1 或 2 哪个延迟更低，或 Jetson 能达到相同 FPS。

## 风险与后续验证

| 风险 | 当前处理 | 真实设备上的下一步 |
|---|---|---|
| 推理慢于采集 | 有界队列与背压 | 固定视频下测 P50/P95 与丢帧策略 |
| 4GB 统一内存压力 | 无真实内存测量 | 按 [4GB 实验配置](../../examples/ch14_face_detection/platforms/jetson-orin-nano/experiment-config.md) 记录峰值内存与 swap |
| 输入不稳定 | 当前只模拟 | 固定视频先验收，摄像头/RTSP 单独报告 |
| 后处理成为瓶颈 | 已独立阶段和指标 | 记录 NMS、绘制、编码或结果交接的时间 |

## 第一阶段结论

第一阶段已经完成一个从 CPU 正确性、并行问题建模、Host 控制面、算子优化、模拟部署到系统设计的最小闭环。下一阶段应只选择一个真实专题，例如 Jetson 固定视频基线或昇腾/CANN，而不是同时扩展全部平台。
