
### 访问静态文件

方法一：static 文件夹

在 Flask 中，**`static/` 目录是专门用于存放静态文件（如图片、CSS、JavaScript 等）** 的默认文件夹，所有放在 `static/` 目录下的文件都可以通过 `http://127.0.0.1:5000/static/...` 直接访问。
```
project/
│── static/                  # Flask 默认静态文件目录
│   ├── img/                 
│       ├── 1.png
|   ├── html/
|       ├── 1.html
│── app.py                   # Flask 服务器
```

方法二：`send_from_directory()` 提供静态资源
```python
from flask import send_from_directory
IMAGE_FOLDER=r"path/to/img"
@app.route('/img/<filename>')
def serve_image(filename):
	return send_from_directory(IMAGE_FOLDER, filename)
```
