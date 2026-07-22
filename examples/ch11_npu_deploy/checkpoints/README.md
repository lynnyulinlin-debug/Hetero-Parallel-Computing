# Checkpoints

这个目录用于存放训练好的模型权重或外部下载的 checkpoint。

推荐约定：

- 按模型名建子目录
- 保留原始框架权重
- 不把导出后的中间工件放在这里

示例：

```text
checkpoints/
└── resnet18/
    └── model.pth
```
