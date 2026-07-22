# 第11章 NPU/TPU：AI专用加速器

> 这一章解决什么问题：理解 AI 专用加速器与 GPU 的本质区别，掌握“模型 → 部署到 NPU”的完整工具链流程，并能把真实模型接进部署模板。

本章拆成五个部分，建议按顺序阅读：

1. [11.1-11.2 硬件特点与设计哲学](11-1-hardware-and-differences.md)
2. [11.3 模型转换流程](11-2-conversion-pipeline.md)
3. [11.4 量化原理](11-3-quantization.md)
4. [11.5 部署实战](11-4-deployment-practice.md)
5. [11.6-11.9 调试、FAQ 与选型](11-5-debugging-faq-selection.md)

`examples/ch11_npu_deploy` 仍然是教学脚手架和接口冻结示例，不是某家厂商 SDK 的完整产品级实现；它的价值在于提前固定接入边界、目录约定和检查项。

最好的阅读顺序是先看 11.1-11.2 建立差异感，再看 11.3-11.5 理解部署链路，最后用 11.6-11.9 把“会部署”与“会排错、会选型”连起来。这样读，既不会一上来就陷入脚本细节，也不会把本章误解成单纯的工具说明。

## 子节入口

- [11.1-11.2 硬件特点与设计哲学](11-1-hardware-and-differences.md)
- [11.3 模型转换流程](11-2-conversion-pipeline.md)
- [11.4 量化原理](11-3-quantization.md)
- [11.5 部署实战](11-4-deployment-practice.md)
- [11.6-11.9 调试、FAQ 与选型](11-5-debugging-faq-selection.md)

