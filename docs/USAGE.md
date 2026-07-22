# 使用说明

> `USAGE` 只负责最短阅读路径、最小运行命令和基础维护原则；项目总览看 `README`，环境部署看 `ENVIRONMENT`，接手优先级看 `HANDOVER`。

## 快速阅读

1. 先看 [前言](00-preface.md)。
2. 再看第三篇和第四篇目录页。
3. 先跑 `examples/ch09_vec_add`，再看第10章。
4. 再看第11章、第14章、第15章和第16章。

角色建议：

- 算法工程师：第8章 → 第11章 → 第14章
- 软件工程师：第一篇 → 第四篇
- 产品经理：第1章 → 第8章 → 第11章 → 第16章
- 系统工程师：第13章 → 第14章 → 第15章 → 第16章

## 运行示例

### CPU-only

```bash
cd examples/ch09_vec_add && make run
cd examples/ch09_reduce && make run
```

### GPU

```bash
cd examples/ch09_vec_add && make USE_CUDA=1 run
cd examples/ch09_reduce && make USE_CUDA=1 run
```

### NPU

```bash
cd examples/ch11_npu_deploy && make run
```

### 端到端流水线

```bash
cd examples/ch14_face_detection
python3 pipeline.py --backend cpu --frames 3
python3 pipeline.py --backend gpu --source list --input-list models/source.list --frames 3
python3 pipeline.py --backend gpu --source camera --frames 3
python3 pipeline.py --backend gpu --source stream --frames 3
python3 pipeline.py --backend npu --frames 3
```

重点看：

- `vec_add` / `reduce` 的 `PASS`
- NPU 脚手架生成的 `artifacts/*`
- 第14章的 `fps`、`capture_q_max`、`infer_q_max`、`stage_ms`
- 第15章的 DAG、同步点和队列边界
- 第16章的场景化选型结论

## 维护

- 新增示例先补 README、FAQ 和最小可运行命令。
- 硬件依赖强的示例保留模拟输入或占位脚手架。
- 入口 CLI 尽量稳定。
