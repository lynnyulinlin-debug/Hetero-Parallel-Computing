# Task 5 参考记录：模拟部署报告

> 本记录只验证部署报告的身份、字段和失败语义。它不包含 Jetson、RK3588、昇腾或其他设备的真实推理数据。

## 执行路径

```bash
ctest --test-dir build -L python --output-on-failure
python3 examples/ch11_classification_infer/04_infer.py \
  --report-json /tmp/task5-classification.json \
  --output /tmp/task5-classification.txt
python3 examples/ch11_classification_infer/04_infer.py --mode real
```

Python 契约测试通过。模拟报告包含：

```text
execution_mode: simulated
metric_source: teaching_profile
backend: rknn
model_exists / test_dir_exists / test_list_exists: false
```

固定的 `latency_ms`、`top1_acc`、`top5_acc` 是教学画像，不能作为 RK3588、Jetson 或昇腾的实际结果。脚本同时输出模型、测试目录和列表不存在的 warning；这说明模拟路径不隐藏设备前置条件。

## 真实模式失败语义

未提供现有模型、测试数据和厂商运行时报告时，`--mode real` 返回失败并说明：

```text
real mode requires existing model, test data and --real-report from a vendor runtime
```

这比输出虚构延迟更重要。真实模式应在目标设备上导入可追溯的外部运行时报告，并记录平台、运行时版本、模型、输入、延迟、精度与失败条件。

## Task 5 结论

- 已验证：模拟部署链路、报告 schema 和真实模式的缺失前置条件失败。
- 未验证：模型转换、量化误差、真实 runtime、板端延迟、吞吐和功耗。
- 后续：Jetson 使用[固定视频基线](../../examples/ch14_face_detection/platforms/jetson-orin-nano/video-baseline.md)；昇腾走[第二阶段 CANN 专题](../phase2/ascend-cann.md)。
