# 第一阶段验收包

> 本页把 [Task 0-6](../TASKS.md) 转换为可执行、可提交的验收清单。完成全部七项即建立第一阶段的工程判断力；不要求读完全部 16 章，也不要求拥有 GPU、Jetson 或 NPU。

## 使用方式

1. 先按 Task 0 建立 `build/` 并保存命令输出。
2. 每完成一个 Task，先按[任务记录卡](task-record-guide.md)整理“输入 -> 动作 -> 证据 -> 结论”，再在[验收记录模板](acceptance-template.md)中填写一个条目；可保存到个人实验记录或项目的 `reports/` 目录。
3. 没有硬件时采用本页的 CPU、`simulated` 与设计路径，报告中明确写出未做硬件验证。
4. 完成后用“通过条件”逐项复核。性能数字只在同一机器、同一输入和同一测量口径下比较。

## 统一构建

```bash
cmake -S . -B build -DHETEROPAR_ENABLE_TESTS=ON
cmake --build build
ctest --test-dir build -L cpu --output-on-failure
```

CPU CTest 全部通过是 Task 0 的前置条件。下面的正则命令只筛选本任务相关测试；完整回归仍以 `ctest -L cpu` 为准。

各 Task 的填写方式见 [Task 0 基线记录](task0-baseline-reference.md)、[Task 1 问题模型记录](task1-vec-add-reference.md)、[Task 2 Host 控制面记录](task2-host-control-reference.md)、[Task 3 Device 映射记录](task3-gpu-operator-reference.md)、[Task 4 优化记录](task4-optimization-reference.md)、[Task 5 部署记录](task5-deployment-reference.md) 和 [Task 6 系统设计记录](task6-system-design-reference.md)。这些记录是写法示范，不是跨机器性能基准。

## Task 0：运行基线

| 项目 | 要求 |
|---|---|
| 最短命令 | 上述统一构建命令 |
| 保留证据 | 操作系统、C 编译器版本、CMake 版本、`OMP_NUM_THREADS` 或实际线程配置、CTest 结果 |
| 验收问题 | 为什么数值正确性通过，不能推出并行实现一定更快？ |
| 通过条件 | `ctest -L cpu` 全部通过，且能区分正确性结果与性能结果 |

参考表达见 [Task 0 基线记录](task0-baseline-reference.md)。

## Task 1：并行问题模型

| 项目 | 要求 |
|---|---|
| 最短命令 | `./build/ch09_vec_add 4099` |
| 保留证据 | 一次非对齐输入规模的输出，以及自选负载的并行单元、共享数据、同步点和首要瓶颈 |
| 验收问题 | 为什么 `vec_add` 适合数据并行，而不需要复杂任务图？串行部分怎样限制总加速比？ |
| 通过条件 | 能给出负载拆分和瓶颈假设，且没有把线程数量直接等同于加速比 |

参考表达见 [`vec_add` 问题模型记录](task1-vec-add-reference.md)。

## Task 2：Host 控制面

| 项目 | 要求 |
|---|---|
| 最短命令 | 任选 `./build/ch04_task_pool` 或 `./build/ch05_openmp_blur`，再运行 `ctest --test-dir build -R 'ch0[4-7]_.*_cpu_correctness' --output-on-failure` |
| 保留证据 | 一个主实验的 Host 执行记录，以及 SIMD、Host-Device 重叠各一条机制观察 |
| 验收问题 | 主实验的任务如何提交、执行和收口？为什么小批量搬运和过碎同步会损失收益？ |
| 通过条件 | 能为一个循环标注 `shared`、`private` 或 `reduction`，并指出至少一个同步收口点 |

## Task 3：基础 Device 映射

| 项目 | 要求 |
|---|---|
| 最短命令 | `ctest --test-dir build -R 'ch09_(vec_add|reduce)_cpu_correctness' --output-on-failure` |
| 可选硬件命令 | `cmake -S . -B build-cuda -DHETEROPAR_ENABLE_CUDA=ON`，随后构建并运行 `ctest --test-dir build-cuda -L cuda --output-on-failure` |
| 保留证据 | `vec_add` 或 `reduce` 的数据流、线程映射、正确性结果、输入规模和首要瓶颈判断 |
| 验收问题 | CPU 参考、CUDA kernel 与 Host-Device 数据传输各自验证什么？ |
| 通过条件 | 无硬件时明确写“Device 映射为设计推导，未进行 GPU 硬件验证”；有 CUDA 设备时记录实际设备和 CUDA 运行结果 |

## Task 4：受限算子优化

| 项目 | 要求 |
|---|---|
| 最短命令 | `ctest --test-dir build -R ch10_fused_softmax_cpu_correctness --output-on-failure` |
| 观察命令 | `./build/ch10_fused_softmax 17 31` |
| 保留证据 | 一个优化前后对照：实现差异、正确性、输入、时间口径、改善项和未改善瓶颈 |
| 验收问题 | 为什么消除中间张量可能改善端到端时间，却不保证所有输入都更快？ |
| 通过条件 | 区分 kernel 时间与端到端时间；不以一次运行或固定加速比作为结论 |

## Task 5：部署契约

| 项目 | 第一阶段要求 | 第二阶段硬件专题 |
|---|---|---|
| 最短命令 | `python3 examples/ch11_classification_infer/04_infer.py --report-json /tmp/task5.json`，再执行 `ctest --test-dir build -L python --output-on-failure` | Jetson 视频/TensorRT、RK3588 或昇腾/CANN 的真实报告 |
| 补充观察 | 主动以 `--mode real` 运行一次，观察缺少模型、输入或厂商报告时的失败结果 | 使用真实模型、输入和运行时生成可归档报告 |
| 保留证据 | `execution_mode: simulated`、报告字段和一个真实模式失败结果 | 平台、运行时版本、模型、输入、延迟 P50/P95、吞吐和失败条件 |
| 通过条件 | 不把教学画像称为硬件测量 | `execution_mode: real`，且报告可追溯到实际设备和输入 |

`real` 模式只导入并归档外部运行时报告；它不替代真实 TensorRT 或厂商 NPU 推理。第一阶段只验收左列；右列是第二阶段的真实硬件要求。

## Task 6：小型系统设计

| 项目 | 要求 |
|---|---|
| 最短命令 | `python3 examples/ch14_face_detection/pipeline.py --backend cpu --frames 3 --report-json /tmp/face.json` |
| 选择场景 | 边缘视觉或云端注意力流水线二选一；无需同时实现 |
| 保留证据 | 任务图、CPU/GPU/NPU 分工、队列边界、同步点、指标、一个风险与备选方案 |
| 验收问题 | 当前场景先受吞吐、单请求延迟、数据搬运、功耗还是维护成本约束？为什么？ |
| 通过条件 | 至少提出一个可复测改动，并标出哪些指标来自模拟、哪些来自真实设备 |

## 完成检查

- [ ] Task 0 的 CPU CTest 通过并已记录环境。
- [ ] Task 1-3 有一个基础算子记录，硬件状态明确。
- [ ] Task 4 有优化对照，包含未改善的瓶颈。
- [ ] Task 5 有部署报告，`simulated` 与 `real` 不混用。
- [ ] Task 6 有一个小型系统设计和可复测改动。
- [ ] 所有性能结论附带输入、实现、测量口径和硬件状态。

完成上述检查后，再从 [Task 0-6 的第二阶段入口](../TASKS.md#第二阶段入口) 选择专项，而不是按章节机械继续。
