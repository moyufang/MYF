
#### PIL
python 自带图像库 `PIL` 集成了图像抓取，不过效率较低，本机全屏截取大概 0.06s 一帧。
```python
import PIL.ImageGrab as gb

def capture_screen(x1, y1, x2, y2):
	img = gb.grab(bbox=(x1, y1, x2-x1, y2-y1))

```

### 

### win32
```python
import time
import win32gui, win32ui, win32con
import numpy as np
import cv2 as cv
import ctypes

def get_window_handle(name=''):
  if not name: win_handle = win32gui.GetDesktopWindow()
  else: 
    win_handle = 0
	# 获取屏幕窗口，即所有窗口的共同祖先
    all_hd = win32gui.GetDesktopWindow(); hd_list = []
    # 遍历子窗口
    win32gui.EnumChildWindows(all_hd, lambda hwnd,param:param.append(hwnd), hd_list)
    for hwnd in hd_list:
      title = win32gui.GetWindowText(hwnd)
      if title and win32gui.IsWindowVisible(hwnd) and name in title:
        win_handle = hwnd
  if win_handle == 0:
    assert False, "Find not handle.\n"
    return None
  else:
    return win32gui.GetWindowRect(win_handle), win_handle

def print_all_handle():
  all_hd = win32gui.GetDesktopWindow()
  hd_list = []
  win32gui.EnumChildWindows(all_hd, lambda hwnd,param:param.append(hwnd), hd_list)
  for hwnd in hd_list:
    title = win32gui.GetWindowText(hwnd)
    # 输出所有窗口的句柄及其标题
    # 找到你想要的窗口，将其标题复制下来即可
    print('handle:%d title:%s'%(hwnd,title))

class Graber:
  '''
  src : 'int' or 'windows dc' : 窗口来源
  region : [width:int, height:int] : 区域
  '''
  def __init__(self, src, region):
    self.set_region(region)
    self.handle = src if isinstance(src, int) else 0
    self.dc = win32gui.GetWindowDC(src) if isinstance(src, int) else src
    self.sdc = win32ui.CreateDCFromHandle(self.dc)
    self.memdc = self.sdc.CreateCompatibleDC()
    self.bmp = win32ui.CreateBitmap()
    self.bmp.CreateCompatibleBitmap(self.sdc, self.width, self.height)
    self.memdc.SelectObject(self.bmp)
  def set_region(self, region):
    self.region = region
    self.width = region[2]-region[0]
    self.height = region[3]-region[1]
  def shot(self):
    #使用 DirectX、OpenGL 或 Vulkan 渲染的窗口（如游戏、视频播放器、浏览器）会绕过 GDI 直接在显存中绘制，GDI 无法捕获其内容。
    #PrintWindow 通知目标窗口重绘
    ctypes.windll.user32.PrintWindow(self.handle, self.sdc.GetSafeHdc(), 2)
    self.memdc.BitBlt(
      (0, 0),          # 目标起点坐标
      (self.width, self.height), # 复制区域尺寸
      self.sdc,              # 源设备上下文
      (0, 0),          # 源起点坐标
      win32con.SRCCOPY # 光栅操作码（直接复制）
    )
    arr = self.bmp.GetBitmapBits(True)
    img = np.fromstring(arr, dtype='uint8') #'RGBA' as color format
    img.shape = (self.height, self.width, 4)
    return img
  def delete(self):
    self.dc.DeleteDC()
    self.sdc.DeleteDC()
    win32gui.DeleteObject(self.bmp.GetHandle())

print_all_handle()

#将窗口标题复制到此
window_name = '酷狗音乐'
rect, handle = get_window_handle(window_name if 1 else '')
# win32gui.SetWindowPos(handle, 0, 100, 100, 1000, 1000, 0)
print("handle: ", handle)
print("rect:", rect, " (w,h):", rect[2]-rect[0], rect[3]-rect[1])
graber = Graber(handle, rect)

n = 100
total_time, total_count = 0, 0
for i in range(n):
  start_time = time.time()
  img = graber.shot()
  total_time += time.time()-start_time; total_count += 1
  # cv.imshow('w', cv.resize(img, None,fx=0.5,fy=0.5))
  # k = cv.waitKey(100) & 0xFF
  # if k == ord('q'): break
  # else: continue
  
# 作者机实测
# 2560*1440 : time_interval:3.339518 per:0.033395
# 1280:720 : time_interval1.668354 per:0.016684
print("time_interval:%.6lf per:%.6lf"%(total_time, total_time/total_count))
```

### mss 跨平台高性能截图库 

```python
import time
import mss
import cv2
import numpy as np

# 指定截取区域（例如全屏）
rect = [0, 0, 2560, 1440]

with mss.mss() as sct:
  # 获取所有显示器信息
  monitors = sct.monitors
  primary_monitor = monitors[1]  # 主显示器
  
  # 截取主显示器全屏
  
  n = 100
  total_time, total_count = 0, 0 
  for i in range(n):
    start_time = time.time()
    screenshot = sct.grab(primary_monitor)
    frame = np.array(screenshot)[rect[0]:rect[2],rect[1]:rect[3]]
    total_time += time.time()-start_time; total_count += 1
# 作者机实测
# 2560*1440 : time_interval:3.334210 per:0.033342
# 1280:720 : time_interval:3.339807 per:0.033398
print("time_interval:%.6lf per:%.6lf"%(total_time, total_time/total_count))

```