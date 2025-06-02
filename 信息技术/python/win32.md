[win32gui documentation](https://timgolden.me.uk/pywin32-docs/win32gui.html) 
打印所有正在运行的窗口的信息
```python
def print_all_handle():
	# 获取屏幕窗口，即所有窗口的共同祖先
	all_hd = win32gui.GetDesktopWindow()
	hd_list = []
	# 遍历子窗口
	win32gui.EnumChildWindows(all_hd, lambda hwnd,param:param.append(hwnd), hd_list)
	for hwnd in hd_list:
		print('handle:%d title:%s'%(hwnd,win32gui.GetWindowText(hwnd)))
```

根据 windows title 获取窗口的 handle 及其窗口矩阵
```python
def get_window_handle(name):
    win_handle = win32gui.FindWindow(None, name)
    # 获取窗口句柄
    if win_handle == 0:
        assert False, "Find not handle.\n"
        return None
    else:
        return win32gui.GetWindowRect(win_handle), win_handle

window_name = '逍遥模拟器'
(x1, y1, x2, y2), handle = get_window_handle(window_name)
# 将窗口置顶
win32gui.SetForegroundWindow(handle)
```

```python
# 将窗口的左上角移动到 (x,y)，大小设置为 （width,height)
# 需要管理员权限
win32gui.MoveWindow(hwnd, x, y, width, height, True)
```

```python
# 判断句柄是否存在
win32gui.IsWindow(handle)
```