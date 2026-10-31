# 第11章 AI 加速器与部署链路

> 目录页：先读 `intro.md`，再按顺序阅读正文小节。
> 返回 [本篇目录](../README.md)。

## 前置条件

- 已完成第8章的 Device 判断；可先使用 Python 教学脚手架，真实部署还需要对应厂商 SDK、模型、校准数据和设备。

## 导读

- [intro.md](./intro.md)

## 正文小节

- [11.1-11.2 硬件特点与设计哲学](./01-hardware-and-differences.md)
- [11.3 模型转换流程](./02-conversion-pipeline.md)
- [11.4 量化原理](./03-quantization.md)
- [11.5 部署实战](./04-deployment-practice.md)
- [11.6-11.9 调试、FAQ 与选型](./05-debugging-faq-selection.md)

## 配套实验

- [图像分类推理与量化部署 `ch11_classification_infer`](../../../examples/ch11_classification_infer/README.md)：第11章的单模型推理闭环实验。
- [NPU 模型部署 `ch11_npu_deploy`](../../../examples/ch11_npu_deploy/README.md)：第11章的部署脚手架与接口冻结示例。

## 完成产出

- 一份部署记录：模型接口、量化方式、转换产物、运行时验证结果和证据等级（教学模拟或真实设备）。
