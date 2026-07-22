# 12.2 OpenCL Host 端运行时 API 流程

OpenCL 需要掌握的重点，不是某个 API 名字，而是 Host 端的编程流程。  
这条流程回答的是：**Host 如何发现设备、组织资源、提交任务、取回结果。**

一个最小的 Host 端流程通常包括下面这些步骤：

1. **平台发现**：先找到可用的 OpenCL platform。
2. **设备选择**：从 platform 里挑出 CPU、GPU 或 FPGA 设备。
3. **Context 创建**：把设备纳入同一个上下文，作为后续资源管理边界。
4. **Command Queue 创建**：建立命令队列，决定任务提交和执行顺序。
5. **Program 构建**：把 kernel 源码编译成可执行程序对象。
6. **Kernel 创建**：从 program 中取出具体 kernel。
7. **Buffer 分配**：准备输入输出缓冲区。
8. **Kernel 参数设置**：把缓冲区和标量参数绑定到 kernel。
9. **Kernel 提交**：把任务放入队列。
10. **结果回读**：把设备侧结果取回 Host。
11. **资源释放**：释放 buffer、kernel、program、queue 和 context。

这个流程和 CUDA Host 端有相似的结构，但 OpenCL 更强调“通用运行时对象”的组织方式。  
把这条链路看清楚，就能明白 OpenCL 不只是写 kernel，更是 Host 端围绕 runtime 组织任务。

### 12.2.1 Host 端最容易混淆的点

OpenCL 初学者最容易在这几件事上混：

- platform 和 device 的关系。
- context 和 queue 的作用边界。
- buffer 和 host memory 的区别。
- kernel 构建和 kernel 提交是两件事。
- 任务提交顺序和实际执行顺序未必完全相同。

所以这一章如果要讲 OpenCL，重点应该放在 Host 端运行时流程，而不是只堆 API 名字。
