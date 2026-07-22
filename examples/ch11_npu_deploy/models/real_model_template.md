# 真实模型接入模板

## 1. 模型信息

- 模型名称：
- 框架：
- 任务类型：
- 训练权重位置：`checkpoints/<model_name>/...`

## 2. 输入输出约定

- 输入张量名：
- 输出张量名：
- 输入尺寸：`N,C,H,W`
- 预处理：
  - resize:
  - normalize:
  - batch:

## 3. 导出约定

- 导出命令：
```bash
python3 01_export_onnx.py \
  --model <model_name> \
  --checkpoint checkpoints/<model_name>/<checkpoint_file> \
  --input-shape 1,3,224,224 \
  --input-name input \
  --output-name logits \
  --output artifacts/model.onnx \
  --report artifacts/export_report.json
```

## 4. 量化约定

- 后端：`rknn` / `ascend` / `tflite`
- 校准集位置：`data/calib/`
- 校准列表：`data/calib/list.txt`
- 量化命令：
```bash
python3 02_quantize.py \
  --backend <backend> \
  --onnx artifacts/model.onnx \
  --calib-dir data/calib \
  --calib-list data/calib/list.txt \
  --output artifacts/model.quant \
  --report artifacts/quant_report.json
```

## 5. 推理与验证

- 测试集位置：`data/test/`
- 测试清单：`data/test/list.txt`
- 验证指标：
  - top-1 / top-5
  - latency
  - accuracy delta

- 推理命令：
```bash
python3 04_infer.py \
  --backend <backend> \
  --model artifacts/model.quant \
  --test-dir data/test \
  --test-list data/test/list.txt \
  --output artifacts/infer_report.txt \
  --report-json artifacts/infer_report.json
```

## 6. 接入检查清单

- [ ] checkpoint 能被原框架加载
- [ ] 导出 ONNX 成功
- [ ] 校准列表可读
- [ ] 量化产物生成
- [ ] 推理报告生成
- [ ] 结果和基线模型可对比
