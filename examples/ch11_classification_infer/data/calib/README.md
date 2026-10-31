# 校准集说明

`list.txt` 里每行写一个校准样本路径。

示例：

```text
samples/calib_0001.jpg
samples/calib_0002.jpg
samples/calib_0003.jpg
```

量化脚本会读取 `list.txt` 并统计样本数，用于记录 PTQ 校准过程。
