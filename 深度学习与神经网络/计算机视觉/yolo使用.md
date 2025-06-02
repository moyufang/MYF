
预训练模型 `YOLOv8n` / `YOLOv8s` / `YOLOv8m` / `YOLOv8l` / `YOLOv8x`

数据集格式

```
your_dataset/
├── train/
│   ├── images/   # 训练图像
│   └── labels/   # 训练标注
├── val/
│   ├── images/   # 验证图像
│   └── labels/   # 验证标注
├── data.yaml
```

训练配置文件

```python
# Train/val/test sets as 1) dir: path/to/imgs, 2) file: path/to/imgs.txt, or 3) list: [path/to/imgs1, path/to/imgs2, ..]
path: ../datasets/coco8 # dataset root dir (absolute or relative; if relative, it's relative to default datasets_dir)
train: images/train # train images (relative to 'path') 4 images
val: images/val # val images (relative to 'path') 4 images
test: # test images (optional)

# Classes (80 COCO classes)
names:
    0: person
    1: bicycle
    2: car
    # ...
    77: teddy bear
    78: hair drier
    79: toothbrush
```

##### 源代码

`ultralytics\nn\tasks.py` 中的 `parse_model()` 实质性的构建了模型。 

