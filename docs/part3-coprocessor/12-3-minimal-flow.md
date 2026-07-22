# 12.3 一个最小流程示意

为了让读者快速建立心智模型，可以把 OpenCL Host 流程理解成下面这个顺序：

```text
platform -> device -> context -> queue -> program -> kernel -> buffer -> enqueue -> readback
```

如果用 `vec_add` 这样的例子来理解，Host 端做的事情就是：

- 把输入数据准备好。
- 把计算任务交给设备。
- 等待或同步任务完成。
- 把结果取回并校验。

这个最小示意的作用，不是替代真实代码，而是让读者知道 OpenCL Host 端的“骨架”长什么样。

### 12.3.1 从这个流程里看什么

重点不是记住每个 API 的函数名，而是理解三件事：

1. Host 端负责组织和调度。
2. Device 端负责并行计算。
3. 两者之间的边界由 context、queue 和 buffer 定义。

只要这三点清楚，后面看具体代码时就不容易迷路。
