# 附录C 术语表（中英对照）

> 首次出现时正文已加粗+括注英文，此处汇总速查。v1.0 持续补全。

| 中文 | 英文 | 简释 |
|------|------|------|
| 主处理器 | Host | 通常指 CPU，负责控制流与数据准备 |
| 协处理器 | Device | GPU/NPU 等，负责大规模并行计算 |
| 数据并行 | Data Parallelism | 同一操作作用于大量数据 |
| 任务并行 | Task Parallelism | 不同任务并发执行 |
| 映射/归约 | Map / Reduce | 基本并行模式 |
| 页锁定内存 | Pinned / Page-locked Memory | 加速 H2D/D2H 传输的主机内存 |
| 算术强度 | Arithmetic Intensity | 计算量与数据传输量之比 |
| 带宽受限 | Memory-bound | 性能受内存带宽限制 |
| 计算受限 | Compute-bound | 性能受算力限制 |
| 线程束 | Warp | CUDA 中 32 线程的硬件调度单位 |
| 共享内存 | Shared Memory | 块内线程共享的高速片上内存 |
| 占用率 | Occupancy | SM 上活跃 warp 的比例 |
| 分支发散 | Branch Divergence | warp 内线程走不同分支导致串行化 |
| 存储体冲突 | Bank Conflict | 共享内存并发访问被串行化 |
| 流 | Stream | CUDA 中的异步操作队列 |
| 统一内存 | Unified Memory | Host/Device 共享、按需迁移的内存 |
| 脉动阵列 | Systolic Array | NPU 常用的矩阵计算结构 |
| 训练后量化 | PTQ (Post-Training Quantization) | 免训练量化 |
| 量化感知训练 | QAT (Quantization-Aware Training) | 训练中模拟量化 |
| 有向无环图 | DAG | 任务依赖建模 |
