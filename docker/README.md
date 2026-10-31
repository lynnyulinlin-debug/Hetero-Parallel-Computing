# 容器复现

> 容器固定编译器、CMake 与 Python 标准库运行环境；它不能替代 GPU 驱动、Jetson 系统镜像或厂商 NPU SDK。环境选择见 [ENVIRONMENT](../docs/ENVIRONMENT.md)。

## 镜像职责

| 镜像 | 构建时验证 | 默认运行验证 | 不保证的内容 |
|---|---|---|---|
| `heteropar:cpu` | CPU 构建、`cpu|python` CTest | 再次运行 `cpu|python` CTest | CUDA、Jetson、NPU 真实性能 |
| `heteropar:cuda` | CPU/Python CTest，编译 CUDA target | CPU/Python CTest，再运行 CUDA 标签测试 | 无 GPU 时 CUDA 测试会跳过；不代表已完成硬件验收 |

两个 Dockerfile 都不安装 `numpy` 或厂商 Python 包：当前仓库级 CPU/Python 契约测试只依赖标准库。需要运行特定模型或厂商 SDK 时，在对应设备环境按项目 README 安装。

## CPU 镜像

```bash
docker build -f docker/Dockerfile.cpu -t heteropar:cpu .
docker run --rm heteropar:cpu
```

第二条命令应通过 `ctest -L 'cpu|python'`。要进入容器排查：

```bash
docker run --rm -it --entrypoint bash heteropar:cpu
```

## CUDA 镜像

前置条件：宿主机 NVIDIA 驱动可用，并已安装 NVIDIA Container Toolkit。镜像构建会编译 CUDA target，但不在 `docker build` 阶段运行 GPU 测试。

```bash
docker build -f docker/Dockerfile.cuda -t heteropar:cuda .
docker run --rm --gpus all heteropar:cuda
```

默认命令先执行 CPU/Python CTest，再执行 `ctest -L cuda`。没有把 GPU 传入容器时，CUDA 测试按仓库约定跳过；这只能证明镜像可构建，不能证明 kernel 已在设备上通过。

## 边界

- 不将 JetPack、TensorRT engine、视频素材、RKNN、CANN 或昇腾驱动加入通用镜像。
- Jetson 固定视频实验使用设备上的 JetPack/TensorRT，见 [Jetson 实验配置](../examples/ch14_face_detection/platforms/jetson-orin-nano/experiment-config.md)。
- RK3588 与昇腾专题各自记录厂商工具链、模型和运行时版本，容器只可作为 Host 侧辅助环境。
- `.dockerignore` 排除本地构建目录、媒体素材、生成工件和 Git 元数据；不要依赖未复制进镜像的宿主机文件。

## 发布前检查

1. 本地先运行 `python3 tests/check_markdown_links.py`。
2. 构建 CPU 镜像并运行默认 CTest。
3. 有 NVIDIA GPU 时，构建 CUDA 镜像并使用 `--gpus all` 运行默认 CTest。
4. 将镜像 tag、基础镜像版本、构建命令和测试输出写入发布记录。
