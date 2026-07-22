# 真实模型接入指南

这份指南说明如何把一个真实模型接入 `ch11_npu_deploy`。
目标不是把厂商 SDK 写死，而是先把接入顺序、目录边界和产物约定固定下来。

## 1. 先选模型类型

- 图像分类：参考 [resnet18_example.md](resnet18_example.md)
- 目标检测：参考 [detection_example.md](detection_example.md)
- 新模型起步：先看 [real_model_template.md](real_model_template.md)

如果模型类别接近分类或检测之外，先把输入输出张量、预处理和后处理写清，再开始导出。

## 2. 先放权重，再补清单

建议目录：

```text
ch11_npu_deploy/
├── checkpoints/<model_name>/model.pth
├── data/calib/list.txt
├── data/test/list.txt
└── artifacts/
```

- `checkpoints/` 只放训练权重或原始 checkpoint。
- `data/calib/list.txt` 只放校准样本清单。
- `data/test/list.txt` 只放测试样本清单。
- `artifacts/` 只放导出、量化、推理产物。

## 3. 按固定顺序接入

1. 填模型信息和输入输出约定。
2. 跑 `01_export_onnx.py`，确认导出参数正确。
3. 跑 `02_quantize.py`，确认校准集和后端参数正确。
4. 跑对应的转换脚本。
5. 跑 `04_infer.py`，确认测试清单和结果报告正确。

## 4. 接口边界

- `01_export_onnx.py` 负责“模型 → ONNX”。
- `02_quantize.py` 负责“ONNX → 量化产物”。
- `03_convert_*` 负责“量化产物 → 厂商格式”。
- `04_infer.py` 负责“板上推理 + 指标报告”。

## 5. 接入时最容易出错的点

- checkpoint 路径写对了，但原框架加载不了。
- 导出 ONNX 时输入尺寸、输入名、输出名不一致。
- 校准集不代表真实分布，量化后精度掉得太多。
- 测试集清单和实际样本目录不一致。
- 后端支持的算子和导出图不一致。

## 6. 最后确认

接入完成后，至少要能回答这四个问题：

- 这个模型输入是什么？
- 这个模型怎么导出？
- 这个模型怎么量化？
- 这个模型怎么验证结果？
