# NPU 平台环境

本页只说明边缘 NPU 对照路径。Jetson 是边缘 GPU 主线后的第一真实硬件项目；RK3588 等 NPU 用于比较量化、转换与运行时约束。昇腾/CANN 的数据中心路径见 [第二阶段昇腾专题](../phase2/ascend-cann.md)。当前支持部署链路与报告契约，不承诺无硬件环境下完成真实厂商后端。

## 可选平台

- RK3588 + RKNN Toolkit / Runtime
- 其他具有明确量化、转换和运行时接口的平台

## 最小验证

```bash
cd examples/ch11_classification_infer && make run
cd ../ch11_npu_deploy && make run
```

默认输出是教学模拟报告。真实模式必须提供厂商运行时报告，并记录模型、校准数据、后端、平台、运行时版本、延迟/精度口径和失败条件。

RK3588 的系统协同语境见 [第14章案例3](../part04-systems/chapter14-end-to-end-cases/03-rk3588-rv1828.md)。
