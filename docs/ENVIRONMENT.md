# 环境部署说明

> 目的：给项目提供一份统一的环境部署与验证入口，避免 CPU / GPU / NPU / 容器信息分散在多个文档里。

## 适用范围

- `docs/` 里的教程正文
- `examples/` 里的可运行示例
- `docker/` 里的容器化复现思路

## 环境分层

### 1. 基础环境

适合验证 CPU 示例、OpenMP 示例和大部分文档内容。

- Linux
- GCC 9+
- `make`
- 可选：`cmake`

建议验证命令：

```bash
cd examples/ch09_vec_add && make run
cd examples/ch09_reduce && make run
```

通过标准：

- 能看到串行与 OpenMP 的计时对比
- 能看到 `PASS` 或等价的结果校验输出

### 2. GPU 环境

适合验证 CUDA 示例和性能分析流程。

- NVIDIA GPU
- CUDA Toolkit 11+
- `nvcc`
- 与 CUDA 版本匹配的 NVIDIA 驱动
- 可选：Nsight Systems、Nsight Compute

建议验证命令：

```bash
cd examples/ch09_vec_add && make USE_CUDA=1 run
cd examples/ch09_reduce && make USE_CUDA=1 run
```

通过标准：

- CUDA 目标可以编译
- 机器有 GPU 时，CUDA 路径能正常运行
- 没有 GPU 时，CPU 路径仍可正常完成并跳过 CUDA 测试

### 3. NPU 环境

适合验证第11章和端到端案例中的部署链路。

NPU 环境依赖具体硬件与厂商 SDK，当前项目按“模板 + 接入边界”方式维护，不要求所有读者都能在本地跑通真实板端推理。

可选目标平台：

- RKNN / 瑞芯微平台
- TFLite delegate 平台
- Ascend / CANN 平台

建议验证命令：

```bash
cd examples/ch11_npu_deploy && make run
```

通过标准：

- 导出、量化、转换、推理四个阶段的脚手架能串起来
- `artifacts/` 中能产出报告或中间工件
- 没有真实板卡时，仍能理解完整部署链路

### 4. 容器环境

适合希望稳定复现、避免编译器和运行时漂移的场景。

当前仓库里容器部分主要是说明框架，目标是后续提供：

- `Dockerfile.cpu`
- `Dockerfile.cuda`

容器环境的价值是：

- 固定编译器版本
- 固定 CUDA 运行时
- 降低依赖漂移

## 推荐验证矩阵

| 场景 | 推荐环境 | 验证入口 |
|---|---|---|
| CPU 教学与回归 | Linux + GCC + `make` | `examples/ch09_vec_add`、`examples/ch09_reduce` |
| GPU 教学与回归 | NVIDIA GPU + CUDA Toolkit | `examples/ch09_vec_add`、`examples/ch09_reduce` |
| NPU 接入模板 | 对应开发板 + 厂商 SDK | `examples/ch11_npu_deploy` |
| 端到端流水线 | CPU/GPU/NPU 按后端切换 | `examples/ch14_face_detection/pipeline.py` |

## 最小验证流程

1. 先验证 CPU-only 路径。
2. 再验证 GPU 路径是否可编译、可运行。
3. 然后验证 NPU 部署模板是否能产出中间工件。
4. 最后验证端到端流水线的后端切换和指标输出。

## 维护原则

- 新增示例先补最小可运行命令。
- 硬件依赖强的示例必须保留模拟输入或占位脚手架。
- 验证命令尽量稳定，不要频繁改入口参数。
- 任何新环境要求都应先写进这里，再同步到对应示例 README。

## 相关文档

- [使用说明](USAGE.md)
- [交接说明](HANDOVER.md)
- [附录A 实验环境搭建指南](appendix/A-environment-setup.md)
- [容器环境](../docker/README.md)
