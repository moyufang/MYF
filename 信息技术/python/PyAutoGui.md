[让所有GUI都自动化-PyAutoGUI（GUI自动化工具）](https://zhuanlan.zhihu.com/p/574137942)
PyAutoGui 用于 pc 端的操作模拟，包括模拟鼠标的移动、点击、拖拽，键盘按键输入、按住操作，以及鼠标+键盘的热键同时按住等操作，可以说手能动的都可以。

PyAutoGUI 有几个特点：
1. 移动鼠标并单击其他应用程序的窗口。
2. 向应用程序发送点击输入（例如，填写表格）。
3. 截取屏幕截图，并给出一个图像（例如，一个按钮或复选框），然后在屏幕上找到它。
4. 找到应用程序的窗口，然后移动、调整大小、最大化、最小化或关闭它（目前仅限 Windows）。
5. 显示警报和消息框。

值得注意的是，PyAutoGUI 只负责模拟键鼠输入，并未集成键鼠操作的监听功能。
#### 安装

1、Windows 环境下

PyAutoGUI 没有任何依赖，因为它用 Python 的 ctypes 模块，所以不需要 pywin32。

```text
pip3 install pyautogui
```

2、macOS 环境下

PyAutoGUI 需要 PyObjC 运行 AppKit 和 Quartz 模块。

```text
sudo pip3 install pyobjc-core
sudo pip3 install pyobjc
sudo pip3 install pyautogui
```

3、Linux 环境下

PyAutoGUI 需要 python-xlib（Python 2）、python3-Xlib（Python 3）

```text
sudo pip3 install python3-xlib
sudo apt-get scrot
sudo apt-get install python-tk
sudo apt-get install python3-dev
sudo pip3 install pyautogui
```

#### 鼠标类

```python
# 获取当前鼠标坐标，范围二维向量
pyautogui.position()
# 屏幕分辨率
pyautogui.size()
# 判断坐标是否在屏幕上
pyautogui.onScreen(x,y)
```

```python

# 用num_seconds秒的时间把光标移动到(x, y)位置
pyautogui.moveTo(x, y, duration=num_seconds)
# 用num_seconds秒的时间把光标的
# X轴（水平）坐标移动xOffset，
# Y轴（竖直）坐标向下移动yOffset。
pyautogui.moveRel(xOffset, yOffset, duration=num_seconds)

### 鼠标移动（缓动/渐变）
# 开始很慢，不断加速
pyautogui.moveTo(100, 100, 2, pyautogui.easeInQuad)
# 开始很快，不断减速
pyautogui.moveTo(100, 100, 2, pyautogui.easeOutQuad)
# 开始和结束都快，中间比较慢
pyautogui.moveTo(100, 100, 2, pyautogui.easeInOutQuad)
# 一步一徘徊前进
pyautogui.moveTo(100, 100, 2, pyautogui.easeInBounce)
# 徘徊幅度更大，甚至超过起点和终点
pyautogui.moveTo(100, 100, 2, pyautogui.easeInElastic)

### 鼠标拖拽

# 按住鼠标左键，把鼠标拖拽到(100, 200)位置
pyautogui.dragTo(100, 200, button='left')
# 按住鼠标左键，用2秒钟把鼠标拖拽到(300, 400)位置
pyautogui.dragTo(300, 400, 2, button='left')
# 按住鼠标右键，用2秒钟把鼠标拖拽到(30,0)位置
pyautogui.dragTo(30, 0, 2, button='right')

### 鼠标点击
# 先移动到(100, 200)再单击
pyautogui.click(x=100, y=200, duration=2)
# 右键
pyautogui.click(button='right')
# 双击左键
pyautogui.click(clicks=2)
# 两次单击之间停留0.25秒
pyautogui.click(clicks=2, interval=0.25)
# 三击右键
pyautogui.click(button='right', clicks=2, interval=0.25)
# 鼠标左键按下再松开
pyautogui.mouseDown(); pyautogui.mouseUp()
# 按下鼠标右键
pyautogui.mouseDown(button='right')
# 移动到(100, 200)位置，然后松开鼠标右键
pyautogui.mouseUp(button='right', x=100, y=200)

### 鼠标滚动
# 向上滚动10格
pyautogui.scroll(10)
# 向下滚动10格
pyautogui.scroll(-10)
# 移动到(100, 100)位置再向上滚动10格
pyautogui.scroll(10, x=100, y=100)
```

#### 键盘
`typewrite()`函数可以实现字符输入。要在两次输入间增加时间间隔，可以用 interval 参数。此函数只能用于单个字符键，不能按 SHITF 和 F1 这些功能键。

`KEYBOARD_KEYS` 获取按键名称。

`press()`函数把 `pyautogui.KEYBOARD_KEYS` 里面按键对应的字符串输入进去。事实上 `press()` 相当于 `KeyDown()` 和 `KeyUp()` 的组合。

`hotkey()`函数来绑定若干键盘热键。像 Ctrl-S 或 Ctrl-Shift-1 都可以用此函数来实现。

```python
### typewrite
# 输入Hello world!
pyautogui.typewrite('Hello world!')
# 每次输入间隔0.25秒，输入Hello world!
pyautogui.typewrite('Hello world!', interval=0.25)
# 多个键也可以
pyautogui.typewrite(['a', 'b', 'c', 'left', 'backspace', 'enter', 'f1'], interval=0.5)

#返回一个列表，里面是所有键对应的字符串
print(pyautogui.KEYBOARD_KEYS)

### press
# ENTER键
pyautogui.press('enter')
# F1键
pyautogui.press('f1')
# 左方向键
pyautogui.press('left')
# 用数组把一组键传入
pyautogui.press(['left', 'left', 'left'])

###键盘热键
# 全选
pyautogui.hotkey('ctrl', 'a') 
# 复制
pyautogui.hotkey('ctrl', 'c') 
# 粘贴
pyautogui.hotkey('ctrl', 'v') 

```

### 截屏

```python

# 截取整个屏幕
# 返回一个Pillow/PIL的Image对象
pyautogui.screenshot()
pyautogui.screenshot('Desktop.png')
# 选区截屏
pyautogui.screenshot(region=(0,0,300,400))

```























