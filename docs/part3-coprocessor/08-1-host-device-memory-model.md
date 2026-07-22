# 8.1 主-从内存模型：Host ↔ Device

- Host（CPU）与 Device（GPU/NPU）各有独立内存，通过 PCIe/总线连接。
- 数据必须显式搬运（除统一内存外）；这是异构编程一切复杂性的根源。
- 〔图占位：Host 内存 / Device 内存 / 互联总线示意〕

