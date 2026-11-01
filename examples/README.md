# 示例索引

这一页汇总当前仓库里的可运行示例。第一阶段的权威顺序见 [Task 0-6](../docs/TASKS.md)，本页按“Host 控制面 -> GPU / 云端算子 -> AI 加速器、边缘与系统”索引项目；第二阶段硬件专题见 [专题路线](../docs/phase2/README.md)。

## Host 控制面实验

| 示例 | 对应章节 | 角色 | 说明 |
|---|---|---|---|
| [ch04_task_pool](./ch04_task_pool/README.md) | 第4章 | 线程池与任务队列 | 看任务如何进入队列、被唤醒、被工作线程消费 |
| [ch05_openmp_blur](./ch05_openmp_blur/README.md) | 第5章 | 循环并行实验 | 观察 OpenMP 如何把规则循环拆给多个线程 |
| [ch06_simd_layernorm](./ch06_simd_layernorm/README.md) | 第6章 | SIMD 向量化实验 | 看 LayerNorm 如何从标量循环变成向量化循环 |
| [ch07_host_overlap](./ch07_host_overlap/README.md) | 第7章 | Host 调度与重叠 | 看 pinned memory、async copy 和 stream overlap |

## GPU / 云端算子实验

| 示例 | 对应章节 | 角色 | 说明 |
|---|---|---|---|
| [ch09_vec_add](./ch09_vec_add/README.md) | 第9章 / 第10章 / 第13章 | 带宽受限基准 | 先看串行 vs OpenMP vs CUDA，理解传输和内存带宽的影响 |
| [ch09_reduce](./ch09_reduce/README.md) | 第9章 / 第10章 / 第13章 | 同步与归约基准 | 看共享内存、原子和分层归约如何影响性能 |
| [ch09_gemm](./ch09_gemm/README.md) | 第9章 / 第10章 / 第13章 | 计算受限基准 | GEMM 适合看分块、共享内存和算力利用 |
| [ch10_fused_softmax](./ch10_fused_softmax/README.md) | 第10章 / 第13章 / 第14章 / 第15章 | 算子融合实验 | 观察中间张量消除如何改变带宽和端到端效率 |

## AI 加速器、边缘与系统实验

| 示例 | 对应章节 | 角色 | 说明 |
|---|---|---|---|
| [ch11_classification_infer](./ch11_classification_infer/README.md) | 第11章 | 单模型推理与量化实验 | 看导出、量化、转换和推理如何组成完整闭环 |
| [ch11_npu_deploy](./ch11_npu_deploy/README.md) | 第11章 | NPU 部署脚手架 | 把导出、量化、转换和板端验证拆成可维护阶段 |
| [ch14_face_detection](./ch14_face_detection/README.md) | 第14章 / 第15章 | 端到端流水线 | 实时视频流人脸检测，适合看阶段边界、队列和后端切换 |
| [ch14_attention_pipeline](./ch14_attention_pipeline/README.md) | 第14章 / 第15章 | 注意力流水线实验 | 看 QK^T、softmax、PV 如何被组织进一个教学型流水线 |

## 跨平台选读实验

| 示例 | 对应章节 | 角色 | 说明 |
|---|---|---|---|
| [ch12_opencl_vec_add](./ch12_opencl_vec_add/README.md) | 第12章 | OpenCL 最小闭环 | 只在多后端需求明确时，观察 platform/device、buffer、kernel 与结果回读 |

## 章节 -> 项目 -> 作用

| 章节 | 项目 | 作用 |
|---|---|---|
| 第4章 | [ch04_task_pool](./ch04_task_pool/README.md) | 线程池与任务队列，讲任务生命周期、唤醒与收尾 |
| 第5章 | [ch05_openmp_blur](./ch05_openmp_blur/README.md) | OpenMP 循环并行，讲共享/私有、调度策略和负载均衡 |
| 第6章 | [ch06_simd_layernorm](./ch06_simd_layernorm/README.md) | SIMD 向量化，讲自动向量化和 `omp simd` |
| 第7章 | [ch07_host_overlap](./ch07_host_overlap/README.md) | Host 调度与异步重叠，讲 pinned memory、stream overlap |
| 第9章 | [ch09_vec_add](./ch09_vec_add/README.md)、[ch09_reduce](./ch09_reduce/README.md)、[ch09_gemm](./ch09_gemm/README.md) | 带宽受限、归约与计算受限的基础算子基准 |
| 第10章 | [ch10_fused_softmax](./ch10_fused_softmax/README.md) | 算子融合实验，讲中间张量消除和带宽优化 |
| 第11章 | [ch11_classification_infer](./ch11_classification_infer/README.md)、[ch11_npu_deploy](./ch11_npu_deploy/README.md) | 单模型推理闭环与部署脚手架 |
| 第14章 | [ch14_face_detection](./ch14_face_detection/README.md)、[ch14_attention_pipeline](./ch14_attention_pipeline/README.md) | 端到端流水线与注意力流水线实验 |

## 按阶段使用

1. 第一阶段 Task 0-2：从 [ch04_task_pool](./ch04_task_pool/README.md) 到 [ch07_host_overlap](./ch07_host_overlap/README.md)，完成 Host 控制面观察。
2. 第一阶段 Task 3-4：从 [ch09_vec_add](./ch09_vec_add/README.md)、[ch09_reduce](./ch09_reduce/README.md) 到 [ch10_fused_softmax](./ch10_fused_softmax/README.md)，完成算子与测量对照。
3. 第一阶段 Task 5-6：使用 [ch11_classification_infer](./ch11_classification_infer/README.md) 的模拟报告和 [ch14_face_detection](./ch14_face_detection/README.md) 的模拟流水线。
4. 第二阶段：按目标平台进入 [Jetson 固定视频基线](./ch14_face_detection/platforms/jetson-orin-nano/video-baseline.md)、[昇腾/CANN 专题](../docs/phase2/ascend-cann.md) 或其他专题。

## 按项目查阅

1. 先从 [ch04_task_pool](./ch04_task_pool/README.md) 开始，熟悉统一计时和校验口径。
2. 再看 [ch05_openmp_blur](./ch05_openmp_blur/README.md) 和 [ch06_simd_layernorm](./ch06_simd_layernorm/README.md)，建立“循环并行 vs 向量化”的判断。
3. 然后看 [ch07_host_overlap](./ch07_host_overlap/README.md)，把 Host 调度、传输和重叠串起来。
4. 云端 GPU 路径：继续看 [ch09_gemm](./ch09_gemm/README.md)、[ch10_fused_softmax](./ch10_fused_softmax/README.md) 和 [ch14_attention_pipeline](./ch14_attention_pipeline/README.md)，把基础能力放回高吞吐算子和 LLM 场景。
5. 边缘 AI 路径：直接看 [ch11_classification_infer](./ch11_classification_infer/README.md)，再对照 [ch11_npu_deploy](./ch11_npu_deploy/README.md) 与 [ch14_face_detection](./ch14_face_detection/README.md)，理解部署链路和输入流水线。

## 维护原则

- 每个示例都尽量保持最小可运行闭环。
- 每个示例都应有统一的计时和结果校验。
- 新增示例优先挂到这里，再挂到对应章节目录页。
