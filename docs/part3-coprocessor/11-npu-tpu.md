# 第11章 NPU/TPU：AI专用加速器

> 本章说明 AI 专用加速器与 GPU 的区别，并给出“模型 → 部署到 NPU”的完整工具链流程。

本章分成五个部分：

1. [11.1-11.2 硬件特点与设计哲学](11-1-hardware-and-differences.md)
2. [11.3 模型转换流程](11-2-conversion-pipeline.md)
3. [11.4 量化原理](11-3-quantization.md)
4. [11.5 部署实战](11-4-deployment-practice.md)
5. [11.6-11.9 调试、FAQ 与选型](11-5-debugging-faq-selection.md)

`examples/ch11_npu_deploy` 仍然是教学脚手架和接口冻结示例，不是某家厂商 SDK 的完整产品级实现；它的价值在于提前固定接入边界、目录约定和检查项。

11.1-11.2 先建立硬件差异，11.3-11.5 再看部署链路，11.6-11.9 处理排错与选型。

## 子节入口

- [11.1-11.2 硬件特点与设计哲学](11-1-hardware-and-differences.md)
- [11.3 模型转换流程](11-2-conversion-pipeline.md)
- [11.4 量化原理](11-3-quantization.md)
- [11.5 部署实战](11-4-deployment-practice.md)
- [11.6-11.9 调试、FAQ 与选型](11-5-debugging-faq-selection.md)
