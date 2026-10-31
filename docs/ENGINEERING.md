# 代码与测试框架

> 本文件定义示例代码的构建、测试和性能测量边界。教程章节职责见 [ARCHITECTURE](ARCHITECTURE.md)，最小运行命令见 [USAGE](USAGE.md)。本规范先冻结目标，后续实现按优先级逐步接入。

## 1. 目标与边界

- `CMake + CTest` 是仓库级构建和回归测试的唯一权威入口。
- 每个示例目录的 `Makefile` 保留为读者就地运行的快捷入口，不再各自定义不同的构建策略。
- `test` 只验证确定性正确性和契约；`benchmark` 才测性能。性能不进入通过/失败门槛。
- CUDA、NPU 和真实设备测试必须可选；CPU 正确性测试必须在无 GPU、无开发板的环境中运行。
- 教学模拟可以进入自动化测试，但必须输出 `simulated` 身份，不能伪装为设备实测。

## 2. 目录职责

```text
examples/
  common/                 共享计时、校验和设备辅助头文件
  ch04_* ... ch14_*       章节配套示例，保留现有稳定路径
tests/                    仅放跨示例测试脚本和契约测试
cmake/                    可复用 CMake 函数、编译选项和测试注册逻辑
```

- 不移动现有 `examples/chXX_*` 路径，章节文档和外链继续使用它们。
- C/CUDA 示例保留“串行参考实现 + 并行候选实现 + 驱动程序”的教学结构。
- Python 部署示例保留“导出 -> 量化 -> 转换 -> 推理”的阶段结构，但要区分模拟和真实后端。
- `examples/common/verify.h` 负责数值正确性，`bench.h` 只负责统计计时；二者不得互相替代。

## 3. 构建模型

### 3.1 CMake 选项

| 选项 | 默认值 | 含义 |
|---|---|---|
| `HETEROPAR_BUILD_HOST` | `ON` | 构建纯 CPU / Host 示例 |
| `HETEROPAR_ENABLE_CUDA` | `OFF` | 查找 CUDA Toolkit 并构建 CUDA 变体 |
| `HETEROPAR_BUILD_SIMULATED_DEPLOYMENT` | `ON` | 注册 Python 教学模拟与契约测试 |
| `HETEROPAR_ENABLE_TESTS` | `ON` | 启用 CTest |

### 3.2 目标规则

- 每个可编译示例都有一个稳定 target，例如 `ch04_task_pool`、`ch09_gemm`、`ch10_fused_softmax`。
- CPU target 必须显式声明 C 标准、警告、OpenMP 或 pthread 依赖，不能依赖编译器隐式默认值。
- CUDA target 只在 `HETEROPAR_ENABLE_CUDA=ON` 时出现，并与 CPU target 共用同一份参考实现和验证逻辑。
- `-march=native` 不作为默认参数；需要本机向量化时，通过 `HETEROPAR_NATIVE_ARCH=ON` 明确启用。
- Makefile 的 `run`、`test`、`benchmark` 最终调用同一套已定义的可执行文件或参数，避免和 CMake 漂移。

## 4. 测试模型

| 层级 | 运行环境 | 内容 | CTest 标签 |
|---|---|---|---|
| CPU 正确性 | 任意 Linux + C 编译器 | 小规模确定性输入、串行参考对照、边界参数 | `cpu;correctness` |
| CUDA 正确性 | 有 CUDA Toolkit 与可用设备 | CPU 参考对照、内存错误和 kernel 结果 | `cuda;correctness;hardware` |
| Python 契约 | Python 3 | 阶段输入输出、报告 schema、失败状态 | `python;contract` |
| 模拟部署 | 无设备 | 明确 `simulated` 的阶段串联与报告字段 | `simulated;contract` |
| 性能基准 | 指定基准机 | warmup、中位数、环境记录和人工比较 | `benchmark;manual` |

测试必须满足以下规则：

- 失败返回非零退出码；不能只打印 `FAIL` 或 warning。
- 每个数值示例至少覆盖一个小尺寸、一个非对齐尺寸和一个默认教学尺寸。
- GPU 不可用时，硬件标签测试可以跳过；CPU 测试不能因此跳过。
- NPU 脚本的真实模式在模型、数据、SDK 或设备缺失时必须失败；模拟模式必须在 JSON 报告写入 `execution_mode: "simulated"`。
- 基准输出至少记录实现名、输入规模、迭代次数和统计口径；不以固定加速比作为 CI 门槛。

## 5. 标准入口

```bash
# CPU 构建与正确性测试
cmake -S . -B build -DHETEROPAR_ENABLE_TESTS=ON
cmake --build build
ctest --test-dir build -L cpu --output-on-failure

# 可选 CUDA 构建与设备测试
cmake -S . -B build-cuda -DHETEROPAR_ENABLE_CUDA=ON
cmake --build build-cuda
ctest --test-dir build-cuda -L cuda --output-on-failure

# 手工性能测量，不作为 CI 通过条件
./build/ch09_gemm 512
```

章节目录中的 `make run` 仍是就地教学快捷入口；仓库级正确性验证以 `ctest` 为准。

## 6. 实施顺序

1. 已完成：根 `CMakeLists.txt` 纳入九个 CPU C 示例，启用 CTest，建立共享编译选项和 target 命名。
2. 已完成：为第 4-7、9、10、14 章 CPU 可运行示例注册小规模正确性测试；默认规模仍用于手工基准。
3. 已完成：Python 部署项目具备模拟/真实模式、报告 schema 与契约测试。
4. 已完成：接入可选 CUDA target 与硬件标签测试；没有设备时不阻断 CPU CI。
5. 已完成：定义 GitHub Actions 工作流，在 CPU runner 上执行 Markdown 链接检查、CPU 构建/CTest 与 Python 契约测试；本地等价命令已通过。工作流需随下一次提交推送后才能形成远端运行记录；CUDA 使用专用 runner 或手工发布验收。
6. 已完成：CPU/CUDA Dockerfile 在构建阶段复用 CMake/CTest；CPU 默认运行验证 `cpu|python`，CUDA 默认运行验证 CUDA 标签测试。

## 7. 验收标准

- `cmake -S . -B build` 能构建全部 CPU 示例。
- `ctest -L cpu` 覆盖每个 CPU 示例的正确性路径。
- `ctest -L python` 覆盖部署报告的成功与失败语义。
- CUDA/NPU 平台结果可追溯到硬件、软件版本和输入配置。
- 文档中的“可运行”“模拟”“真实部署”“性能结果”分别对应明确命令和可验证证据。

## 8. CI 边界

- 工作流文件为 [`.github/workflows/ci.yml`](../.github/workflows/ci.yml)，触发于 push、pull request 和手工运行。
- `tests/check_markdown_links.py` 只检查仓库内公开 Markdown 的相对文件和标题锚点；内部交接文件不参与公开文档链接门禁。
- CI 运行 `ctest -L 'cpu|python'`，覆盖 9 个 CPU 正确性测试和部署报告契约测试。
- CUDA、Jetson、RK3588、昇腾等真实硬件测试不在通用 GitHub runner 执行；其结果必须由专用 runner 或发布前人工验收提供。

## 9. 容器边界

- [CPU Dockerfile](../docker/Dockerfile.cpu) 固定 CPU 构建与 Python 契约环境，默认命令执行 `ctest -L 'cpu|python'`。
- [CUDA Dockerfile](../docker/Dockerfile.cuda) 在镜像构建时编译 CUDA target；运行时只有以 `--gpus all` 传入设备才可形成 CUDA 硬件证据。
- 通用镜像不携带 JetPack、TensorRT engine、RKNN、CANN、模型权重或视频素材；设备专题各自记录运行时与输入版本。
