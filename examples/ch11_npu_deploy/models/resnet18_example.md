# ResNet18 接入示例

## 1. 模型信息

- 模型名称：resnet18
- 框架：PyTorch
- 任务类型：图像分类
- 训练权重位置：`checkpoints/resnet18/model.pth`

## 2. 输入输出约定

- 输入张量名：`input`
- 输出张量名：`logits`
- 输入尺寸：`1,3,224,224`
- 预处理：
  - resize: 224x224
  - normalize: ImageNet mean/std
  - batch: 1

## 3. 导出约定

```bash
python3 01_export_onnx.py \
  --model resnet18 \
  --checkpoint checkpoints/resnet18/model.pth \
  --input-shape 1,3,224,224 \
  --input-name input \
  --output-name logits \
  --output artifacts/model.onnx \
  --report artifacts/export_report.json
```

## 4. 量化约定

```bash
python3 02_quantize.py \
  --backend rknn \
  --onnx artifacts/model.onnx \
  --calib-dir data/calib \
  --calib-list data/calib/list.txt \
  --output artifacts/model.quant \
  --report artifacts/quant_report.json
```

## 5. 推理与验证

```bash
python3 04_infer.py \
  --backend rknn \
  --model artifacts/model.quant \
  --test-dir data/test \
  --test-list data/test/list.txt \
  --output artifacts/infer_report.txt \
  --report-json artifacts/infer_report.json
```

## 6. 接入检查清单

- [ ] checkpoint 可加载
- [ ] ONNX 可导出
- [ ] 校准列表可读
- [ ] 量化产物可生成
- [ ] 推理报告可生成
