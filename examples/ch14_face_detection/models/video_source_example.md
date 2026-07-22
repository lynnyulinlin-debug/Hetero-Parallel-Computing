# 真实输入源样例

这个样例给第14章的 `--source list` 提供一个可直接照搬的输入文件格式。
当前流水线先把它当作“帧来源列表”使用，后续接真实解码器时可以无缝替换成视频文件路径或摄像头来源。

## 1. 输入文件格式

- 每行一个来源
- 支持空行
- 支持 `#` 注释
- 建议使用相对路径或绝对路径，不要混写

示例：

```text
# 训练/验证素材
samples/video_0001.mp4
samples/video_0002.mp4
samples/video_0003.mp4
```

## 2. 目录约定

建议把真实视频素材放到：

```text
ch14_face_detection/
├── samples/
│   ├── video_0001.mp4
│   ├── video_0002.mp4
│   └── video_0003.mp4
└── models/
    └── source.list
```

`source.list` 只负责列来源，不直接存媒体文件。

## 3. 运行方式

```bash
python3 pipeline.py \
  --backend gpu \
  --source list \
  --input-list models/source.list \
  --frames 3
```

如果想对比不同后端，只替换 `--backend`：

```bash
python3 pipeline.py --backend cpu --source list --input-list models/source.list --frames 3
python3 pipeline.py --backend npu --source list --input-list models/source.list --frames 3
```

## 4. 预期结果

- `source=list` 时，流水线会把输入文件中的条目轮询到每一帧
- 输出会打印：
  - `backend`
  - `source`
  - `fps`
  - `capture_q_max`
  - `infer_q_max`
  - 各阶段平均耗时

## 5. 接入提醒

- 当前版本还没有真正解码视频帧，只是把来源列表和流水线接口先固定下来。
- 如果 `source.list` 缺失或为空，流水线会回退到模拟帧。
- 后续接入真实解码器时，只需要替换 `stages/capture_decode.py` 的实现，`pipeline.py` 的接口尽量保持不变。
