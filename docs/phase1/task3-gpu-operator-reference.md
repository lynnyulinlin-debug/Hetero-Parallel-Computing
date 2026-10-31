# Task 3 参考记录：基础 Device 映射闭环

> 本记录完成“正确性 -> 映射 -> 瓶颈解释”的 CPU-only 部分。2026-10-21 的工作站没有运行 CUDA target；下面的 GPU 内容是实现设计，不是设备测量。

## CPU 对照

```bash
ctest --test-dir build \
  -R 'ch09_(vec_add|reduce)_cpu_correctness' --output-on-failure
./build/ch09_vec_add 4099
./build/ch09_reduce 4099
```

两个正确性测试通过。`4099` 是非对齐的小输入：`vec_add` 和 `reduce` 的 OpenMP 路径都慢于串行参考，说明线程调度和测量固定成本压过了实际工作量。它不用于预测 GPU 加速比。

## `vec_add`：无依赖的数据并行

```text
global index i = blockIdx.x * blockDim.x + threadIdx.x
if i < n: y[i] = a[i] + b[i]
```

| 项目 | 映射与判断 |
|---|---|
| 数据流 | Host 输入 `a,b` -> Device -> kernel 写 `y` -> Host 校验 |
| 线程映射 | 一个 thread 处理一个下标 `i` |
| 内存访问 | 连续线程访问连续元素时，读取和写入应合并访问 |
| 同步 | kernel 内无需跨线程同步；Host 在读取 `y` 前等待 kernel 和回传完成 |
| 首要瓶颈 | 大规模时通常是全局内存带宽；小规模时是 launch 与 Host-Device 传输固定成本 |

## `reduce`：需要分层同步的数据并行

```text
每个 thread 读取一个或多个元素形成局部和
    -> block 内用 shared memory 分层归约并同步
    -> block 结果再用第二阶段 kernel 或原子操作汇合
```

| 项目 | 映射与判断 |
|---|---|
| 数据流 | Host 输入 `x` -> Device 局部归约 -> block 部分和 -> 全局结果 -> Host 校验 |
| 线程映射 | 一个 thread 处理一个或多个元素，先形成局部和 |
| 同步 | block 内每轮 shared-memory 归约需要同步；block 间不能靠单个 kernel 的普通同步完成 |
| 首要瓶颈 | 全局内存读取、同步轮次、分支与最终汇合；原子写过多会形成竞争 |
| 与 `vec_add` 的差异 | 输出位置不是天然独占，必须设计分层归约或受控原子汇合 |

## 验收边界

- 已验证：CPU 串行参考与 OpenMP 候选实现结果一致。
- 未验证：CUDA 编译、设备可见性、kernel 正确性、传输时间、GPU 内存带宽与真实加速比。
- 后续有 NVIDIA GPU 时，使用 `cmake -S . -B build-cuda -DHETEROPAR_ENABLE_CUDA=ON`，再运行 `ctest --test-dir build-cuda -L cuda --output-on-failure`，并在同一输入和口径下补充报告。

第一阶段在无硬件条件下到此已具备 CPU、模拟与映射闭环；Task 4-6 再进入受限优化、部署报告和系统设计。
