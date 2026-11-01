# OpenCL `vec_add`：跨平台 Host 流程

> 对应第12章。该示例只建立 OpenCL 的最小 Host/Device 闭环，不比较 CUDA 性能，也不替代固定平台的原生工具链。

## 前置条件

- OpenCL 1.2 headers、loader 与至少一个 ICD/device；CPU OpenCL 实现也可以。
- 不满足前置时，示例会输出 `OPENCL SKIP`，这不是正确性通过。

## CMake 入口

```bash
cmake -S . -B build-opencl -DHETEROPAR_ENABLE_OPENCL=ON
cmake --build build-opencl --target ch12_opencl_vec_add
ctest --test-dir build-opencl -L opencl --output-on-failure
```

就地教学入口可使用 `make run`；仓库回归仍以 CMake/CTest 为准。

## 观察

1. Host 选择 platform/device，创建 context 与 queue。
2. Host 加载 `vec_add.cl`，创建三个 buffer，提交 kernel 并读回结果。
3. 输出设备名并使用串行参考校验结果。

完成产出是一条可移植性记录：平台/设备、输入规模、正确性结果、是否实际执行，以及继续使用 OpenCL 的维护理由。
