# 实战：NPU 模型部署

> 对应章节：第11.5节。跑通一个图像分类模型的 NPU 部署全流程：模型转换 → 量化 → 推理。

## ⚠️ 硬件依赖说明

NPU 推理强依赖具体硬件与厂商 SDK，**没有对应开发板的读者无法直接运行推理部分**。为此本示例刻意将流程解耦为两层：

- **平台无关层**（可在任意机器运行/阅读）：PyTorch → ONNX 导出、量化校准数据准备、转换脚本的逻辑。
- **平台相关层**（需对应硬件）：ONNX → NPU 格式转换、板上推理。

支持的目标平台（任选其一，按你手头的硬件）：RKNN（瑞芯微）、TFLite delegate、昇腾（Ascend CANN）。

## 状态

✅ 已补齐平台无关层脚手架，并明确平台相关层的接口位置。该示例的目标不是脱离硬件运行完整推理，而是把部署链路拆成可维护的阶段。

```
ch11_npu_deploy/
├── 01_export_onnx.py        # PyTorch 模型 → ONNX（平台无关）
├── 02_quantize.py           # PTQ 量化 + 校准集（平台无关逻辑，调用厂商工具）
├── 03_convert_rknn.py       # ONNX → RKNN（需 rknn-toolkit2）
├── 03_convert_ascend.sh     # ONNX → om（需 ATC 工具）
├── 04_infer.py              # 板上推理 + 精度/耗时对比
├── checkpoints/             # 训练权重与 checkpoint 约定
├── models/                  # 模型说明与接入模板
├── data/
│   ├── calib/               # 校准集说明与样本列表
│   └── test/                # 测试集说明与样本列表
├── artifacts/               # 导出、量化、推理报告输出
└── requirements.txt
```

## 当前可运行内容

- `01_export_onnx.py`：导出阶段的脚手架，生成 ONNX 工件清单和导出报告。
- `02_quantize.py`：量化阶段的脚手架，记录校准集、后端和量化方案。
- `03_convert_rknn.py` / `03_convert_ascend.sh`：转换阶段的后端入口。
- `04_infer.py`：推理阶段的脚手架，输出文本与 JSON 报告。

## 推荐输入约定

- `data/calib/list.txt`：每行一个校准样本路径
- `data/test/list.txt`：每行一个测试样本路径
- `artifacts/`：导出、量化与推理报告输出目录

## 目录约定

- 先补 `data/calib/list.txt` 和 `data/test/list.txt`。
- `artifacts/` 保存中间工件和报告。
- 真实权重放 `checkpoints/`，说明和模板放 `models/`。
- 接入真实模型时，优先看 [models/resnet18_example.md](models/resnet18_example.md) 或 [models/real_model_template.md](models/real_model_template.md)。

## 教学要点

- 模型转换流程（第11.3节）：框架格式 → ONNX 中间表示 → 厂商专有格式。
- 量化原理（第11.4节）：PTQ vs QAT，量化前后精度对比。
- 常见问题（第11.7节）：算子不支持、动态 shape、精度下降的定位思路。
- 性能调试（第11.6节）：NPU 利用率、DMA 带宽、算子耗时分解。

## 建议的执行顺序

1. 先做 `01_export_onnx.py`，把模型和输入尺寸固定下来。
2. 再做 `02_quantize.py`，把校准数据和量化配置固定下来。
3. 再接厂商转换脚本，验证目标格式是否可生成。
4. 最后做 `04_infer.py`，输出精度与耗时对比结果。

## 示例命令

```bash
python3 01_export_onnx.py --model resnet18 --output artifacts/model.onnx --report artifacts/export_report.json
python3 02_quantize.py --backend rknn --onnx artifacts/model.onnx --calib-dir data/calib --calib-list data/calib/list.txt
python3 04_infer.py --backend rknn --model artifacts/model.quant --test-dir data/test --test-list data/test/list.txt
```

## 示例输出

```text
$ make run
python3 01_export_onnx.py --model resnet18 --output artifacts/model.onnx --report artifacts/export_report.json
[export] wrote ONNX artifact manifest to artifacts/model.onnx
[export] wrote export report to artifacts/export_report.json
python3 02_quantize.py --backend rknn --onnx artifacts/model.onnx --calib-dir data/calib --calib-list data/calib/list.txt --output artifacts/model.quant --report artifacts/quant_report.json
[quantize] wrote quantized artifact manifest to artifacts/model.quant
[quantize] wrote quantization report to artifacts/quant_report.json
python3 04_infer.py --backend rknn --model artifacts/model.quant --test-dir data/test --test-list data/test/list.txt --output artifacts/infer_report.txt --report-json artifacts/infer_report.json
[infer] wrote inference summary to artifacts/infer_report.txt
[infer] wrote inference report to artifacts/infer_report.json
```

## 什么时候用哪个文件

- 新模型：先填 [models/real_model_template.md](models/real_model_template.md)
- ResNet18 起步样例：直接看 [models/resnet18_example.md](models/resnet18_example.md)
- 检测模型起步样例：直接看 [models/detection_example.md](models/detection_example.md)
- 真实接入流程：先看 [models/integration_guide.md](models/integration_guide.md)
- 改目录和命令：优先改 README，再改脚本参数

## 动手实验 / 综合作业

将端到端案例（第14章案例1）的人脸检测模型从 GPU 迁移到 NPU，记录遇到的算子兼容问题与精度变化。
