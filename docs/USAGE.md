# 使用说明

> `USAGE` 只负责最短阅读路径、最小运行命令和基础维护原则；项目总览看 [README](../README.md)，环境部署看 [ENVIRONMENT](ENVIRONMENT.md)，章节职责看 [ARCHITECTURE](ARCHITECTURE.md)。

> 章节阅读规则：每章先看章节文件夹下的 `README.md`，再读 `intro.md`，最后按目录页顺序读正文小节。

## 学习入口

1. 第一次学习：从 [第一阶段 Task 0-6](TASKS.md) 开始，按验收包完成最小工程闭环。
2. 已有等价工程基础：从 [第二阶段专题路线](phase2/README.md) 选择一个方向，不按章节线性补课。
3. 查阅某一章：先看该章 `README.md`，再读 `intro.md` 和正文小节。

## 按主题查阅

如果你主要想看某类内容：

- 调度：第2章、第4章、第5章、第7章、第15章
- 云端 GPU / 算子与性能：第8章、第9章、第10章、第13章
- 边缘 GPU：第7章、第9章、第10章、第14章、第16章
- 边缘 NPU 对照：第11章、第14章、第16章
- 可移植性：第12章（选读）

## 最小运行

没有 GPU 或开发板时，先执行 `ctest --test-dir build -L 'cpu|python' --output-on-failure`，再按 [第一阶段验收包](phase1/README.md) 完成 CPU、模拟与设计路径；任务范围见 [无硬件学习路径](TASKS.md#无硬件学习路径)。

### CPU

```bash
cd examples/ch09_vec_add && make run
cd examples/ch09_reduce && make run
cd examples/ch04_task_pool && make run
cd examples/ch05_openmp_blur && make run
cd examples/ch06_simd_layernorm && make run
```

### GPU

```bash
cd examples/ch09_vec_add && make USE_CUDA=1 run
cd examples/ch09_reduce && make USE_CUDA=1 run
cd examples/ch09_gemm && make USE_CUDA=1 run
cd examples/ch10_fused_softmax && make USE_CUDA=1 run
cd examples/ch14_attention_pipeline && make USE_CUDA=1 run
```

### NPU / 部署

```bash
cd examples/ch11_classification_infer && make run
cd examples/ch11_npu_deploy && make run
```

### 端到端流水线

```bash
cd examples/ch14_face_detection
python3 pipeline.py --backend cpu --frames 3
python3 pipeline.py --backend gpu --source list --input-list models/source.list --frames 3
python3 pipeline.py --backend npu --frames 3
```

## 看什么

- `vec_add` / `reduce` / `gemm` 的 `PASS`
- `ch04_task_pool` 的线程池吞吐
- `ch05_openmp_blur` 的线程数和调度效果
- `ch06_simd_layernorm` 的向量化收益
- `ch07_host_overlap` 的分块、传输和重叠
- `ch11_classification_infer` 的导出、量化、推理报告
- `ch14_face_detection` 的 `fps`、`capture_q_max`、`infer_q_max`、`stage_ms`

## 维护原则

- 新增示例先补 README，再补最小可运行命令。
- 硬件依赖强的示例必须保留模拟输入或占位脚手架。
- 模拟脚手架和真实硬件结果必须在输出与报告中明确区分，不能把模拟指标当作设备实测结果。
- 入口 CLI 尽量稳定，改动先同步到 `README`、`USAGE` 和示例 README。

## 相关文档

- [README](../README.md)
- [环境部署说明](ENVIRONMENT.md)
- [术语表](appendix/C-glossary.md)
- [公式与指标表](appendix/E-formulas-and-metrics.md)
- [附录A 实验环境搭建指南](appendix/A-environment-setup.md)
- [容器环境](../docker/README.md)
