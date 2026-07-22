# 目标检测接入示例

## 1. 模型信息

- 模型名称：yolov5s
- 框架：PyTorch
- 任务类型：目标检测
- 训练权重位置：`checkpoints/yolov5s/model.pth`

## 2. 输入输出约定

- 输入张量名：`images`
- 输出张量名：`pred`
- 输入尺寸：`1,3,640,640`
- 预处理：
  - resize: 640x640
  - normalize: 0-1
  - batch: 1

## 3. 导出约定

```bash
python3 01_export_onnx.py \
  --model yolov5s \
  --checkpoint checkpoints/yolov5s/model.pth \
  --input-shape 1,3,640,640 \
  --input-name images \
  --output-name pred \
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

## 6. 检测模型接入提醒

- 检测模型通常还需要后处理：NMS、阈值过滤、框坐标还原。
- 如果后端不支持某些算子，优先考虑简化导出图，再重新验证量化与推理。
- 真实工程里，检测模型比分类模型更容易暴露算子兼容和后处理差异。
