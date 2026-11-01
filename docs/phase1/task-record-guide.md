# 第一阶段任务记录卡

> 每个 Task 只提交一张记录卡。卡片固定为“输入 -> 动作 -> 证据 -> 结论”，避免用命令输出或性能数字替代工程判断。完整字段见[验收记录模板](acceptance-template.md)。

| Task | 输入 | 动作 | 最小证据 | 必须写出的结论 |
|---|---|---|---|---|
| 0 运行基线 | 当前主机、编译器和 CMake 版本 | 构建并运行 CPU CTest | 命令、返回码、CTest 结果 | 正确性通过不等于性能更快 |
| 1 问题模型 | 一个数组计算或自选负载 | 标注并行单元、共享数据和同步点 | `vec_add` 的小输入/单线程、大输入/单线程、大输入/四线程三组输出 | 为什么选数据并行，以及串行部分的收益上限 |
| 2 Host 控制面 | 一个循环或一批小任务 | 运行线程池或 OpenMP 主实验；观察 SIMD 与同步收口 | 任务切分、变量作用域、等待点 | Host 如何提交、执行、收口，哪里可能重叠 |
| 3 Device 映射 | `vec_add` 或 `reduce` | 画出数据流和 grid/block/thread 映射 | CPU 对照结果、内存访问判断 | 映射是设计推导还是已经过 CUDA 设备验证 |
| 4 受限优化 | `fused softmax` 的同一输入 | 对照 unfused 与 fused 实现 | 正确性结果、计时口径、实现差异 | 消除了什么中间张量，哪个瓶颈没有改善 |
| 5 部署契约 | 模拟模型、输入和后端名 | 写出模拟报告，并主动触发一次 real 模式失败 | `execution_mode` 与失败信息 | 模拟报告不能推出真实平台延迟或兼容性 |
| 6 系统设计 | 边缘视觉或云端注意力场景 | 标注任务图、队列、同步点和设备分工 | 流水线报告与一项可复测改动 | 首要约束、风险和下一次测量应验证什么 |

## 最小阅读清单

> 以下链接是第一阶段需要读到的小节，不要求按章通读。章节目录中的扩展小节和所有真实硬件步骤留给第二阶段。

| Task | 只读这些小节 |
|---|---|
| 0 | [环境说明](../ENVIRONMENT.md)、[第1章导读](../part01-foundations/chapter01-why-parallel/intro.md) |
| 1 | [第1章导语](../part01-foundations/chapter01-why-parallel/intro.md)、[性能上限](../part01-foundations/chapter01-why-parallel/02-performance-limits.md)、[并行化判断卡](../part01-foundations/chapter01-why-parallel/04-parallel-mechanics.md)、[第2章导语](../part01-foundations/chapter02-os-parallel-basics/intro.md)、[进程与线程边界](../part01-foundations/chapter02-os-parallel-basics/01-process-vs-thread.md)、[同步原语](../part01-foundations/chapter02-os-parallel-basics/02-synchronization-primitives.md)、[第3章导语](../part01-foundations/chapter03-parallel-patterns/intro.md)、[基本并行模式](../part01-foundations/chapter03-parallel-patterns/01-basic-parallel-modes.md)、[任务与数据并行](../part01-foundations/chapter03-parallel-patterns/02-task-vs-data-parallel.md)、[吞吐与延迟](../part01-foundations/chapter03-parallel-patterns/03-throughput-vs-latency.md) |
| 2 | [线程池](../part02-host-cpu/chapter04-cpu-multithreading/02-thread-pool.md)、[共享与私有变量](../part02-host-cpu/chapter05-openmp/02-sharing-and-privatization.md)、[调度策略](../part02-host-cpu/chapter05-openmp/03-scheduling-strategies.md)、[SIMD 概念](../part02-host-cpu/chapter06-simd/01-simd-concept.md)、[异步提交与同步](../part02-host-cpu/chapter07-cpu-in-hetero/02-async-submission-and-synchronization.md) |
| 3 | [Host-Device 内存模型](../part03-coprocessor/chapter08-coprocessor-overview/01-host-device-memory-model.md)、[线程与内存层次](../part03-coprocessor/chapter09-gpu-cuda/01-thread-and-memory-hierarchy.md)、[kernel 启动](../part03-coprocessor/chapter09-gpu-cuda/02-kernel-launch.md)、[三个基准](../part03-coprocessor/chapter09-gpu-cuda/03-three-benchmarks.md) |
| 4 | [时间线分析](../part03-coprocessor/chapter10-gpu-profiling/01-timeline-analysis.md)、[标准测量流程](../part04-systems/chapter13-performance-methodology/01-standard-flow.md)、[瓶颈与实验](../part04-systems/chapter13-performance-methodology/03-bottlenecks-and-experiments.md) |
| 5 | [转换流程](../part03-coprocessor/chapter11-npu-tpu/02-conversion-pipeline.md)、[量化](../part03-coprocessor/chapter11-npu-tpu/03-quantization.md)、[部署实践](../part03-coprocessor/chapter11-npu-tpu/04-deployment-practice.md) |
| 6 | [人脸检测流水线](../part04-systems/chapter14-end-to-end-cases/01-face-detection.md)、[LLM 调度](../part04-systems/chapter14-end-to-end-cases/02-llm-scheduling.md)、[工作切分](../part04-systems/chapter15-system-coordination/01-work-splitting.md)、[任务图调度](../part04-systems/chapter15-system-coordination/02-dag-scheduling.md)、[场景选择](../part04-systems/chapter16-decision-guide/01-scenario-selection.md) |

## 记录规则

- 输入必须可复现：写明版本、数据规模、模型或场景，不能只写“默认参数”。
- 动作只写本次真正执行或设计的内容；未执行的硬件步骤必须标为“未验证”。
- 证据写返回码、报告字段、图示或测量口径，不引用教学模拟的固定数值作为实测。
- 结论必须同时包含“可以得出什么”和“不能得出什么”。没有 CUDA、Jetson 或 NPU 时，结论停在 CPU 对照、设计推导或模拟契约。

## 示例句式

```text
输入：4099 个 float（1 线程）与 4194304 个 float（1、4 线程），CPU-only。
动作：依次运行三组 vec_add 对照，并将每个元素映射为一个未来的 Device 线程。
证据：三组 CPU 参考与 OpenMP 结果均校验通过；未构建 CUDA target。
结论：该负载具有规则的数据并行结构；当前记录只能比较固定机器、输入和线程配置下的 CPU 现象，不能说明 GPU 加速比。
```
