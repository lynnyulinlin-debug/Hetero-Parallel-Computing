# 附录 C：术语表

> 本表给出本教程使用术语的中英对照和最小工程含义。正文首次出现的解释优先；同一个词在不同厂商工具链中的精确定义应以对应官方文档为准。公式与测量指标见[附录 E](E-formulas-and-metrics.md)。

## 并行与同步

| 中文 | 英文 | 本教程中的含义 |
|---|---|---|
| 并发 | Concurrency | 多个任务在时间上交错推进；不要求同一时刻物理执行。 |
| 并行 | Parallelism | 多个工作单元在同一时间段实际同时执行。 |
| 进程 | Process | 具有独立地址空间和资源边界的执行实体。 |
| 线程 | Thread | 共享进程地址空间的执行单元，需要显式同步。 |
| 竞态条件 | Race Condition | 结果依赖并发操作的非确定时序。 |
| 互斥锁 | Mutex | 保护临界区、同一时刻只允许一个线程进入的同步原语。 |
| 条件变量 | Condition Variable | 让线程等待某个状态成立，并在状态变化后被唤醒。 |
| 屏障 | Barrier | 参与者都到达后才允许继续推进的同步点。 |
| 死锁 | Deadlock | 一组执行单元互相等待，且没有一个能够继续。 |
| 原子操作 | Atomic Operation | 对共享状态不可分割的读改写操作。 |

## 工作拆分与 Host 控制

| 中文 | 英文 | 本教程中的含义 |
|---|---|---|
| 数据并行 | Data Parallelism | 将同一种操作分配到大量独立数据元素。 |
| 任务并行 | Task Parallelism | 并发执行不同类型或不同阶段的任务。 |
| 流水线并行 | Pipeline Parallelism | 让不同输入分别处于多个依赖阶段。 |
| Map / Reduce | Map / Reduce | 对元素独立变换，再合并部分结果的基本模式。 |
| 线程池 | Thread Pool | 复用固定或受控数量工作线程的任务执行器。 |
| 调度 | Scheduling | 决定工作何时、由谁、以多大粒度执行。 |
| 负载均衡 | Load Balancing | 让并行参与者的有效工作尽量均匀，减少尾部等待。 |
| Host | Host | 通常是 CPU 一侧，负责准备数据、提交工作、同步与结果收口。 |
| Device | Device | GPU、NPU 等执行计算工作的加速器一侧。 |
| 同步收口 | Synchronization Join | Host 或任务图等待依赖完成、合并结果的边界。 |

## CPU 向量化

| 中文 | 英文 | 本教程中的含义 |
|---|---|---|
| SIMD | Single Instruction, Multiple Data | 一条指令并行处理多个数据 lane。 |
| 向量寄存器 | Vector Register | 保存多个 lane 的寄存器。 |
| lane | Lane | 向量寄存器中独立处理的元素位置。 |
| 自动向量化 | Auto-vectorization | 编译器证明循环安全后自动生成向量指令。 |
| intrinsics | Intrinsics | 程序员显式组织向量加载、计算和存储的架构相关接口。 |
| 对齐 | Alignment | 数据地址与访问宽度满足硬件或编译器偏好的关系。 |
| 尾部处理 | Tail Handling | 元素数不能整除 lane 数时，对剩余元素的处理。 |

## GPU 与内存系统

| 中文 | 英文 | 本教程中的含义 |
|---|---|---|
| kernel | Kernel | 在 Device 上由大量线程执行的计算函数。 |
| grid / block / thread | Grid / Block / Thread | CUDA 中一次启动的全部工作、线程协作边界和单个执行实例。 |
| warp | Warp | CUDA 中通常以 32 个线程为单位的硬件调度组。 |
| 全局内存 | Global Memory | Device 范围可见、容量大但访问代价较高的内存。 |
| 共享内存 | Shared Memory | block 内线程共享、用于局部复用的片上存储。 |
| 寄存器 | Register | 单线程私有、最快但数量有限的局部状态。 |
| 合并访问 | Coalesced Access | 相邻线程访问连续地址，从而减少内存事务。 |
| 分支发散 | Branch Divergence | 同一 warp 的线程走不同分支，导致部分路径串行执行。 |
| 占用率 | Occupancy | 可活跃 warp 相对硬件容量的比例；不是性能的充分指标。 |
| Stream | Stream | 有序异步操作队列，可与其他 stream 形成重叠。 |
| Event | Event | 标记 Device 工作进度、建立依赖或测量时间的对象。 |
| H2D / D2H | Host-to-Device / Device-to-Host | Host 与 Device 之间的数据传输方向。 |
| 页锁定内存 | Pinned / Page-locked Memory | 不可被操作系统换出的 Host 内存，可支持更稳定的异步传输。 |

## 性能、部署与系统

| 中文 | 英文 | 本教程中的含义 |
|---|---|---|
| 延迟 | Latency | 一次请求或一个工作项从开始到完成所经历的时间。 |
| 吞吐 | Throughput | 单位时间完成的请求、样本、帧或 token 数。 |
| P50 / P95 | Percentile 50 / 95 | 延迟分位数；P95 用于观察尾部延迟。 |
| 批处理 | Batch | 把多个工作项一起提交以提高利用率，可能增加排队延迟。 |
| 背压 | Backpressure | 下游处理不足时，队列、等待或限速向上游传播的约束。 |
| 算术强度 | Arithmetic Intensity | 计算量与数据搬运量之比，用于初步判断带宽或算力瓶颈。 |
| 带宽受限 | Memory-bound | 性能主要受可用内存带宽或数据搬运限制。 |
| 计算受限 | Compute-bound | 性能主要受可用计算吞吐限制。 |
| Roofline | Roofline Model | 以带宽和峰值算力给出性能上界的分析模型。 |
| 模型转换 | Model Conversion | 将模型从训练框架或 ONNX 转为目标运行时可加载的工件。 |
| 量化 | Quantization | 用较低精度表示权重或激活，以降低存储和计算成本。 |
| 校准 | Calibration | 为量化确定范围、尺度等参数的过程。 |
| 运行时 | Runtime | 加载模型、分配资源、提交推理并返回结果的软件层。 |
| 教学模拟 | Simulated | 固定接口和报告格式的教学路径，不代表真实设备结果。 |
