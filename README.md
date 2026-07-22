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

> `README` 只负责总览和快速判断；更具体的运行方式看 `USAGE`，环境部署看 `ENVIRONMENT`。

## 阅读方式

章节分工：第1-3章打底，第4-7章讲主机侧工程，第8-11章讲 GPU/NPU，第12章做补充，第13-16章做收口。当前发布阶段按 `MVP -> Preview -> Alpha -> Full` 递进。

分层标记贯穿全书：**⚡️入门必读** · **🔬进阶探索** · **📋产品经理视角**

| 读者 | 建议路径 |
|------|----------|
| ⚡️ 最短上手路径 | 第3章 → 第8章 → 第9章 → 第10章 → `examples/ch09_vec_add` |
| 算法工程师（AI部署） | 第8章 → 第9章 → 第11章 → 第13章 → 第14章 |
| 📋 产品经理 | 第1章 → 第3章 → 第8章 → 第11章 → 第16章 |

> v0.1 MVP 从第三篇（GPU+NPU 核心）起步。

## 内容入口

> `完成状态` 表示这一章的正文完成度，不是发布阶段。

| 章 | 角色 | 主要内容 | 完成状态 | 正文 |
|---|---|---|---|---|
| 第1章 | 并行基础 | 为什么需要并行计算 | 已完成 | [正文](docs/part1-foundations/01-why-parallel.md) |
| 第2章 | 桥接 | 线程、同步和可见性基础 | 基本完成 | [正文](docs/part1-foundations/02-os-parallel-basics.md) |
| 第3章 | 并行基础 | 常见并行模式与抽象 | 已完成 | [正文](docs/part1-foundations/03-parallel-patterns.md) |
| 第4章 | CPU 主机侧工程 | pthread、线程池与任务组织 | 基本完成 | [正文](docs/part2-host-cpu/04-cpu-multithreading.md) |
| 第5章 | CPU 主机侧工程 | OpenMP 并行写法与调度 | 已完成 | [正文](docs/part2-host-cpu/05-openmp.md) |
| 第6章 | CPU 主机侧工程 | SIMD、向量化和 intrinsic | 已完成 | [正文](docs/part2-host-cpu/06-simd.md) |
| 第7章 | 主机侧收口 | CPU 在异构系统中的职责 | 基本完成 | [正文](docs/part2-host-cpu/07-cpu-in-hetero.md) |
| 第8章 | 协处理器导读 | GPU/NPU 视角和选型前导 | 已完成 | [正文](docs/part3-coprocessor/08-coprocessor-overview.md) |
| 第9章 | GPU 主线 | CUDA 编程模型和内存层次 | 已完成 | [正文](docs/part3-coprocessor/09-gpu-cuda.md) |
| 第10章 | GPU 工具链 | profiler、时间线和瓶颈定位 | 已完成 | [正文](docs/part3-coprocessor/10-gpu-profiling.md) |
| 第11章 | NPU 接入模板 | 模型转换、量化和板端部署 | 已完成 | [正文](docs/part3-coprocessor/11-npu-tpu.md) |
| 第12章 | 跨平台补充 | OpenCL/SYCL 等跨平台模型 | 基本完成 | [正文](docs/part3-coprocessor/12-cross-platform.md) |
| 第13章 | 方法论收口 | 性能分析标准流程 | 已完成 | [正文](docs/part4-systems/13-performance-methodology.md) |
| 第14章 | 系统案例 | 端到端异构案例与实验 | 已完成 | [正文](docs/part4-systems/14-end-to-end-cases.md) |
| 第15章 | 系统协同 | CPU/GPU/NPU 协同和 DAG | 已完成 | [正文](docs/part4-systems/15-system-coordination.md) |
| 第16章 | 决策出口 | 技术选型、成本和产品决策 | 已完成 | [正文](docs/part4-systems/16-decision-guide.md) |

- [前言](docs/00-preface.md)
- **第一篇 预备篇** — 并行计算基础 · [目录](docs/part1-foundations/)
- **第二篇 主处理器篇** — CPU并行编程 · [目录](docs/part2-host-cpu/)
- **第三篇 协处理器篇** — GPU与AI加速器（核心）· [目录](docs/part3-coprocessor/)
- **第四篇 综合篇** — 系统设计与实战 · [目录](docs/part4-systems/)
- [附录](docs/appendix/)

## 运行与环境

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

### 运行环境

详见 [使用说明](docs/USAGE.md) 与 [附录A 实验环境搭建指南](docs/appendix/A-environment-setup.md)。最低要求：Linux + GCC；GPU 需要 CUDA Toolkit，NPU 需要对应开发板。
