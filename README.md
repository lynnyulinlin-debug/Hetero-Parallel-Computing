# 异构并行计算系统实践：从CPU多线程到AI加速器

**HeteroPar: A System-Level Practice to Heterogeneous Parallel Computing**

> 算法工程师与软件开发者的异构计算指南

一份"系统视角、偏软件、广而全"的异构并行计算实践教程。聚焦概念模型、编程模型、工具链与协同工作流，不深入硬件微架构。

一句话总结：这不是单一 CUDA 或 OpenCL 编程手册，而是一门讲解如何把 CPU、GPU、NPU 等异构能力组织成可交付系统的实践教程。

## 适合谁

- **算法工程师**：想把模型跑得更快、部署到加速器上。
- **软件工程师**：需要理解异构系统的编程模型与协同方式。
- **产品经理**：关注选型与决策（看 📋 标记的小节）。

## 使用入口

- [使用说明](docs/USAGE.md)
- [环境部署说明](docs/ENVIRONMENT.md)
- [教程主线图](docs/MAINLINE.md)
- [交接说明](docs/HANDOVER.md)

> `README` 只负责总览和快速判断；更具体的运行方式看 `USAGE`，环境部署看 `ENVIRONMENT`，维护优先级看 `HANDOVER`。

## 发布阶段总览

| 阶段 | 范围 | 目标 |
|---|---|---|
| MVP | 第8-11章 | 主线闭环 |
| Preview | 第1-11章 | 连续阅读 |
| Alpha | 第1-12章 | 加入补充 |
| Full | 第1-16章 | 完整收口 |

## 章节关系

章节职责图见 [教程主线图](docs/MAINLINE.md)。一句话理解：第1-3章打底，第4-7章讲主机侧工程，第8-11章讲 GPU/NPU，第12章做补充，第13-16章做收口。

分阶段发布和交接优先级见 [交接说明](docs/HANDOVER.md)。

## 阅读路径图

分层标记贯穿全书：**⚡️入门必读** · **🔬进阶探索** · **📋产品经理视角**

| 读者 | 建议路径 |
|------|----------|
| ⚡️ 最短上手路径 | 第3章 → 第8章 → 第9章 → 第10章 → `examples/ch09_vec_add` |
| 算法工程师（AI部署） | 第8章 → 第9章 → 第11章 → 第13章 → 第14章 |
| 📋 产品经理 | 第1章 → 第3章 → 第8章 → 第11章 → 第16章 |

> 详细职责图见 [教程主线图](docs/MAINLINE.md)；v0.1 MVP 从第三篇（GPU+NPU 核心）起步。

## 章节总览

| 章 | 定位 | 状态 | 正文 |
|---|---|---|---|
| 第1章 | 原理起点 | 可交付 | [正文](docs/part1-foundations/01-why-parallel.md) |
| 第2章 | 系统机制桥接 | 接近可交付 | [正文](docs/part1-foundations/02-os-parallel-basics.md) |
| 第3章 | 通用模式语言 | 可交付 | [正文](docs/part1-foundations/03-parallel-patterns.md) |
| 第4章 | CPU 多线程工程 | 接近可交付 | [正文](docs/part2-host-cpu/04-cpu-multithreading.md) |
| 第5章 | 低侵入并行 | 接近可交付 | [正文](docs/part2-host-cpu/05-openmp.md) |
| 第6章 | 核内并行优化 | 接近可交付 | [正文](docs/part2-host-cpu/06-simd.md) |
| 第7章 | Host 职责收口 | 可交付 | [正文](docs/part2-host-cpu/07-cpu-in-hetero.md) |
| 第8章 | 第三篇导读 | 可交付 | [正文](docs/part3-coprocessor/08-coprocessor-overview.md) |
| 第9章 | GPU 主线核心 | 可交付 | [正文](docs/part3-coprocessor/09-gpu-cuda.md) |
| 第10章 | 工具链实战 | 可交付 | [正文](docs/part3-coprocessor/10-gpu-profiling.md) |
| 第11章 | 部署与接入模板 | 接近可交付 | [正文](docs/part3-coprocessor/11-npu-tpu.md) |
| 第12章 | 桥接补充 | 接近可交付 | [正文](docs/part3-coprocessor/12-cross-platform.md) |
| 第13章 | 方法论收口 | 可交付 | [正文](docs/part4-systems/13-performance-methodology.md) |
| 第14章 | 系统案例 | 接近可交付 | [正文](docs/part4-systems/14-end-to-end-cases.md) |
| 第15章 | 系统协同 | 可交付 | [正文](docs/part4-systems/15-system-coordination.md) |
| 第16章 | 最终出口 | 可交付 | [正文](docs/part4-systems/16-decision-guide.md) |

## 1-11章内容类型

| 章 | 原理 | 工程 | 动手 | 发布时机 | 说明 |
|---|---|---|---|---|---|
| 第1章 | 高 | 低 | 低 | 先发 | 并行动机与理论上限 |
| 第2章 | 高 | 中 | 中 | 先发 | 操作系统机制与任务队列 |
| 第3章 | 高 | 低 | 低 | 先发 | 并行模式语言 |
| 第4章 | 中 | 高 | 中 | 先发 | pthread 与线程池 |
| 第5章 | 中 | 高 | 高 | 先发 | OpenMP 实战 |
| 第6章 | 中 | 高 | 中 | 先发 | SIMD 与自动向量化 |
| 第7章 | 中 | 高 | 高 | 先发 | Host 职责与数据搬运 |
| 第8章 | 高 | 中 | 低 | 先发 | 协处理器与选型前导 |
| 第9章 | 中 | 高 | 高 | 先发 | CUDA 主线与示例闭环 |
| 第10章 | 中 | 高 | 高 | 先发 | Profiler 与性能定位 |
| 第11章 | 高 | 高 | 中 | 先发 | NPU 原理、接入模板、部署边界 |

> 读法建议：先看第1-3章建立概念，再看第4-7章打通 CPU 工程，再看第8-11章进入 GPU/NPU 与部署模板。  
> 发布建议：第1-11章可以按 `preview/alpha` 先行发布；第2章和第11章后续再统一口径即可。

## 目录

- [前言](docs/00-preface.md)
- **第一篇 预备篇** — 并行计算基础 · [目录](docs/part1-foundations/)
- **第二篇 主处理器篇** — CPU并行编程 · [目录](docs/part2-host-cpu/)
- **第三篇 协处理器篇** — GPU与AI加速器（核心）· [目录](docs/part3-coprocessor/)
- **第四篇 综合篇** — 系统设计与实战 · [目录](docs/part4-systems/)
- [附录](docs/appendix/)

## 代码仓库

```text
docs/        教程正文
examples/    每章可运行示例
docker/      容器环境
CMakeLists.txt 统一构建（可选）
```

### 快速验证

```bash
cd examples/ch09_vec_add && make run   # 无需 GPU，几秒内看到 CPU 串行 vs OpenMP 对比
```

## 运行环境

详见 [使用说明](docs/USAGE.md) 与 [附录A 实验环境搭建指南](docs/appendix/A-environment-setup.md)。最低要求：Linux + GCC；GPU 需要 CUDA Toolkit，NPU 需要对应开发板。
