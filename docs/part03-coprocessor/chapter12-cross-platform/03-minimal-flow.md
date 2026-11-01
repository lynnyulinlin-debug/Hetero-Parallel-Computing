# 12.3 最小可运行 OpenCL `vec_add`

这一节不是为了展示完整工程，而是为了把 OpenCL 的 Host 端骨架先立起来，方便后面映射到真实代码。

为了让读者快速建立心智模型，可以把 OpenCL Host 流程理解成下面这个顺序：

```text
platform -> device -> context -> queue -> program -> kernel -> buffer -> enqueue -> readback
```

![图 12-1：OpenCL `vec_add` 的 Host/Device 流程](../../assets/figures/fig-12-opencl-host-flow.svg)

*图 12-1：Host 负责选择后端、准备 buffer、构建并提交 kernel，Device 执行数据并行计算，结果回到 Host 侧与串行参考校验。平台或设备不可用时应明确跳过，不能输出虚构的跨平台性能。*

如果用 `vec_add` 这样的例子来理解，Host 端做的事情就是：

- 把输入数据准备好。
- 把计算任务交给设备。
- 等待或同步任务完成。
- 把结果取回并校验。

对应项目是 [ch12_opencl_vec_add](../../../examples/ch12_opencl_vec_add/README.md)。它使用相同的 `vec_add` 工作负载：Host 读取 `vec_add.cl`，创建输入/输出 buffer，提交一维 NDRange，读回结果后与串行参考校验。它默认不构建；只有明确存在多后端需求并启用 `HETEROPAR_ENABLE_OPENCL=ON` 时才进入。

### 12.3.1 从这个流程里看什么

重点不是记住每个 API 的函数名，而是理解三件事：

1. Host 端负责组织和调度。
2. Device 端负责并行计算。
3. 两者之间的边界由 context、queue 和 buffer 定义。

只要这三点清楚，后面看具体代码时就不容易迷路。

**小结**
这个项目验证的是 Host 流程、运行时可用性和数值正确性，不验证“OpenCL 比 CUDA 快”。如果目标平台已经固定，正确结论可以是继续使用 CUDA 或厂商运行时；只有多后端交付已成为约束时，才值得继续维护这层接口。
