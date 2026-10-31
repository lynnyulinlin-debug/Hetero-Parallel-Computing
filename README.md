# 异构并行计算系统实践：从CPU多线程到AI加速器

**HeteroPar: A System-Level Practice to Heterogeneous Parallel Computing**

> 面向工程实践的 CPU/GPU/NPU 异构计算教程：从 Host 控制面到云边部署

一份以 CPU、GPU、NPU 为主线的异构并行计算实践教程，重点讲清三者如何分工、协同，以及如何在云端和边缘端落地。

一句话总结：CPU 负责组织任务和数据，GPU 负责通用高吞吐计算，NPU 负责固定模型的高能效推理；云端和边缘端决定它们如何组合。

## 适合谁

- 异构并行开发工程师
- 算法 / AI 部署工程师
- 系统 / 平台工程师
- 技术负责人 / 产品经理

不太适合：

- 完全零基础、还没接触过 Linux / C++ / 并发概念的读者
- 只想查某个 API 用法的人
- 只关心单一平台、完全不需要系统视角的人

## 使用入口

- [使用说明](docs/USAGE.md)
- [环境部署说明](docs/ENVIRONMENT.md)
- [教程架构](docs/ARCHITECTURE.md)
- [代码与测试框架](docs/ENGINEERING.md)
- [第一阶段学习任务](docs/TASKS.md)
- [第二阶段专题路线](docs/phase2/README.md)

> `README` 只负责总览和快速判断；章节职责看 `ARCHITECTURE`，代码框架看 `ENGINEERING`，运行方式看 `USAGE`，环境部署看 `ENVIRONMENT`。

## 阅读方式

章节分工：第1-3章建立基础模型；第4-7章讲 Host 控制面；第8-10章讲 GPU 与云端计算；第11章讲 AI 加速器部署；第12章为跨平台选读；第13-16章讲云边系统、协同和选型。

分层标记贯穿全书：**⚡️入门必读** · **🔬进阶探索** · **📋产品经理视角**

| 读者 | 建议路径 |
|------|----------|
| 第一阶段学习者 | [Task 0-6](docs/TASKS.md)：先完成最小工程闭环，不逐章读完全书 |
| 云端 GPU / AI 工程师 | 第一阶段后选 [云端 GPU 专题](docs/phase2/README.md)：第9、10、13-15章 |
| 边缘 AI 部署工程师 | 第一阶段后走 Jetson 固定视频基线，再按需比较 RK3588 |
| 系统 / 平台工程师 | 第一阶段后选系统工程专题：第13-16章 |
| 📋 产品经理 | 第1章 → 第3章 → 第7章 → 第11章 → 第16章 |

如果你主要想补“调度”，先读第2章、第4章、第5章、第7章和第15章。

## 内容入口

> `完成状态` 表示这一章的正文完成度，不是发布阶段。

| 章 | 角色 | 主要内容 | 完成状态 | 正文 |
|---|---|---|---|---|
| 第1章 | 并行基础 | 为什么需要并行计算 | 已完成 | [目录页](docs/part01-foundations/chapter01-why-parallel/README.md) |
| 第2章 | 桥接 | 线程、同步和可见性基础 | 已完成 | [目录页](docs/part01-foundations/chapter02-os-parallel-basics/README.md) |
| 第3章 | 并行基础 | 常见并行模式与抽象 | 已完成 | [目录页](docs/part01-foundations/chapter03-parallel-patterns/README.md) |
| 第4章 | Host 控制面 | pthread、线程池与任务组织 | 已完成 | [目录页](docs/part02-host-cpu/chapter04-cpu-multithreading/README.md) |
| 第5章 | Host 数据并行 | OpenMP 并行写法、循环调度与数据划分 | 已完成 | [目录页](docs/part02-host-cpu/chapter05-openmp/README.md) |
| 第6章 | Host 执行效率 | SIMD、向量化和数据布局 | 已完成 | [目录页](docs/part02-host-cpu/chapter06-simd/README.md) |
| 第7章 | Host-Device 协同 | 数据放置、搬运、异步提交和同步 | 基本完成 | [目录页](docs/part02-host-cpu/chapter07-cpu-in-hetero/README.md) |
| 第8章 | GPU 执行模型 | 设备内存、协处理器分类与上卡判断 | 已完成 | [目录页](docs/part03-coprocessor/chapter08-coprocessor-overview/README.md) |
| 第9章 | GPU 编程与算子 | CUDA 编程模型、内存层次和基础算子 | 已完成 | [目录页](docs/part03-coprocessor/chapter09-gpu-cuda/README.md) |
| 第10章 | GPU 性能与云端视角 | profiler、时间线和高吞吐服务瓶颈 | 已完成 | [目录页](docs/part03-coprocessor/chapter10-gpu-profiling/README.md) |
| 第11章 | AI 加速器部署 | 模型转换、量化、运行时与设备验证 | 已完成 | [目录页](docs/part03-coprocessor/chapter11-npu-tpu/README.md) |
| 第12章 | 跨平台选读 | OpenCL/SYCL 的可移植性边界 | 已完成 | [目录页](docs/part03-coprocessor/chapter12-cross-platform/README.md) |
| 第13章 | 系统性能方法 | 端到端测量、瓶颈分类和复测 | 已完成 | [目录页](docs/part04-systems/chapter13-performance-methodology/README.md) |
| 第14章 | 云边系统案例 | 云端推理调度、边缘视觉与端到端实验 | 已完成 | [目录页](docs/part04-systems/chapter14-end-to-end-cases/README.md) |
| 第15章 | 系统协同 | CPU/GPU/NPU 协同、任务图和同步 | 已完成 | [目录页](docs/part04-systems/chapter15-system-coordination/README.md) |
| 第16章 | 云边部署决策 | 技术选型、成本和产品决策 | 已完成 | [目录页](docs/part04-systems/chapter16-decision-guide/README.md) |

- [前言](docs/00-preface.md)
- **第一篇 并行计算的系统基础** — 性能约束、并发时序与工作负载模式 · [目录](docs/part01-foundations/README.md)
- **第二篇 Host 并行编程模型与控制面** — 任务、循环、lane 与异构协同 · [目录](docs/part02-host-cpu/README.md)
- **第三篇 GPU 与 AI 加速器篇** — 云端计算与部署主线 · [目录](docs/part03-coprocessor/README.md)
- **第四篇 云边系统篇** — 系统协同、测量与决策 · [目录](docs/part04-systems/README.md)
- [附录：环境、术语、公式与 FAQ](docs/appendix/README.md)
- [示例索引](examples/README.md)

## 运行与环境

```text
docs/        教程正文
examples/    每章可运行示例
docker/      容器环境
CMakeLists.txt 统一构建（可选）
```

### Quickstart：5 分钟完成本地 CPU 验证

前置条件：Linux、CMake 3.18+、支持 OpenMP 的 C 编译器、Python 3。

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build -L 'cpu|python' --output-on-failure
./build/ch09_vec_add 4099
```

预期结果：CPU 正确性测试与部署报告契约通过；`ch09_vec_add` 输出串行与 OpenMP 对照，并显示校验通过。

下一步：有 NVIDIA GPU 看 [CUDA 环境](docs/environment/nvidia-cuda.md)；有 Jetson Orin Nano 看 [Jetson 环境](docs/environment/jetson-orin.md)；需要 NPU 对照看 [NPU 环境](docs/environment/npu.md)。

没有 GPU 或开发板也可以完成 [第一阶段](docs/TASKS.md#无硬件学习路径)：先做 CPU 正确性、Host 调度和算子分析，再运行模拟部署与流水线报告；只有真实性能和厂商 SDK 兼容性需要对应硬件验证。第一阶段完成后，再从[第二阶段专题](docs/phase2/README.md)选择深入方向。

### 运行环境

环境选择见 [环境部署说明](docs/ENVIRONMENT.md)，最短学习路径见 [使用说明](docs/USAGE.md)。
