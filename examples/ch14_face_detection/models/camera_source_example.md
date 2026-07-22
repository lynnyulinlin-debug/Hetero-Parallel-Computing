# 摄像头输入样例

这个样例说明第14章如何把摄像头或真实视频来源写进 `source.list`。
当前流水线已经支持 `--source camera`，这里主要说明单摄像头和多来源混合时的输入格式。

## 1. 推荐写法

```text
# 本地摄像头
camera:0

# USB 摄像头或其他设备
camera:1

# 真实视频文件
samples/demo_0001.mp4
samples/demo_0002.mp4
```

## 2. 使用方式

单摄像头直接运行：

```bash
python3 pipeline.py --backend gpu --source camera --frames 3
```

如果要和视频文件混合使用，把上面的内容保存到 `models/source.list`，然后运行：

```bash
python3 pipeline.py \
  --backend gpu \
  --source list \
  --input-list models/source.list \
  --frames 3
```

## 3. 当前版本的行为

- `camera:*` 和视频路径都会被当作来源标签写入帧元数据。
- 如果后续接入真实解码器，可以让 `camera:0` 直接映射到摄像头句柄。
- 如果 `source.list` 缺失或为空，流水线仍会回退到模拟帧。
