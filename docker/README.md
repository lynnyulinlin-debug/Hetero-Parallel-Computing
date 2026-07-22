# 容器环境

> 提供可复现的运行环境，避免编译器/CUDA/驱动版本漂移。更完整的环境分层与验证矩阵见 [环境部署说明](../docs/ENVIRONMENT.md)。

## 状态

✅ 已补齐两套 Dockerfile 骨架，便于做 CPU / CUDA 环境复现；后续可再按需细化镜像缓存、体积和验证命令。

| 文件 | 用途 |
|------|------|
| `Dockerfile.cpu` | CPU-only：GCC + OpenMP + CMake，零 GPU 依赖，可跑通所有 CPU 示例 |
| `Dockerfile.cuda` | GPU：基于 `nvidia/cuda` 基础镜像，含 nvcc 与 Nsight，需 nvidia-container-toolkit |

## 用法

```bash
# CPU 版
docker build -f docker/Dockerfile.cpu -t heteropar:cpu .
docker run --rm -it heteropar:cpu

# CUDA 版（需主机装好 NVIDIA 驱动与 nvidia-container-toolkit）
docker build -f docker/Dockerfile.cuda -t heteropar:cuda .
docker run --rm -it --gpus all heteropar:cuda
```

> NPU 部署依赖厂商专有 SDK 与硬件，通常无法容器化复现，详见 `examples/ch11_npu_deploy/README.md`。
