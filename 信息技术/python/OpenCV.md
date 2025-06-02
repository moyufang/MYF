[OpenCV-Python Tutorials](https://docs.opencv.org/4.x/d6/d00/tutorial_py_root.html)
### GUI Features
#### 图片的读入和保存

`IMREAD_COLOR,IMREAD_UNCHANGED,IMREAD_GRAYSCALE` 可以指定读入方式
```python
import cv2 as cv
import sys
import numpy as np
import keyboard
 
img1 = cv.imread(r'.\test.png', cv.IMREAD_COLOR)
img2 = cv.imread(r'.\test.png', cv.IMREAD_UNCHANGED)
img3 = cv.imread(r'.\test.png', cv.IMREAD_GRAYSCALE)
print(type(img1))
print(img1.shape, img2.shape, img3.shape)
 
cv.imshow("Title", img1)
keyboard.press_and_release('shift')
k = cv.waitKey(0)
cv.imshow("Title", img2)
k = cv.waitKey(0)
cv.imshow("Title", img3)
k = cv.waitKey(0)
 
#  cv.imwrite("starry_night.png", img)

""" console output 
<class 'numpy.ndarray'>
(390, 324, 3) (390, 324, 4) (390, 324)
"""
```

#### Drawing Functions

```python
img = np.zeros((512,512,3), np.uint8)
cv.line(img,(0,0),(511,511),(255,0,0),5)
cv.rectangle(img,(384,0),(510,128),(0,255,0),3)
cv.circle(img,(447,63), 63, (0,0,255), -1)
# img,(x,y),(a,b),rotate_angle:anti-clockwise,(start_angle,end_angle):clockwise,stroke_width
cv.ellipse(img,(256,256),(100,50),0,0,180,255,-1)
pts = np.array([[10,5],[20,30],[70,20],[50,10]], np.int32)
pts = pts.reshape((-1,1,2))
# img,points_list,whether closed,color	
cv.polylines(img,[pts],True,(0,255,255))
font = cv.FONT_HERSHEY_SIMPLEX
cv.putText(img,'OpenCV',(10,500), font, 4,(255,255,255),2,cv.LINE_AA)

cv.imshow("o", img)
cv.waitKey(0)

```

#### Mouse Event

```python
drawing = False # true if mouse is pressed
mode = True # if True, draw rectangle. Press 'm' to toggle to curve
ix,iy = -1,-1
 
# mouse callback function
def draw_circle(event,x,y,flags,param):
	global ix,iy,drawing,mode,img
	radius = 20e
	if event == cv.EVENT_LBUTTONDOWN:
		drawing = True
		ix,iy = x,y

	elif event == cv.EVENT_MOUSEMOVE:
		if drawing == True:
			if mode == True:
				cv.rectangle(img,(ix,iy),(x,y),(0,255,0),-1)
			else:
				cv.circle(img,(x,y),radius,(0,0,255),-1)
	elif event == cv.EVENT_LBUTTONUP:
		drawing = False
		if mode == True:
			cv.rectangle(img,(ix,iy),(x,y),(0,255,0),-1)
		else:
			cv.circle(img,(x,y),radius,(0,0,255),-1)

img = np.zeros((512,512,3), np.uint8)
cv.namedWindow('image')
cv.setMouseCallback('image',draw_circle)
keyboard.press_and_release('shift')

while(1):
	cv.imshow('image',img)
	k = cv.waitKey(1) & 0xFF
	if k == ord('m'):
		mode = not mode
	elif k == ord('e'):
		break
 
cv.destroyAllWindows()
```

#### TrackBar

```python
nothing = lambda x: x
# Create a black image, a window
img = np.zeros((300,512,3), np.uint8)
cv.namedWindow('image')

# create trackbars for color change
cv.createTrackbar('R','image',0,255,nothing)
cv.createTrackbar('G','image',0,255,nothing)
cv.createTrackbar('B','image',0,255,nothing)

# create switch for ON/OFF functionality
switch = '0 : OFF \n1 : ON'
cv.createTrackbar(switch, 'image',0,1,nothing)
print(chr(27))
keyboard.press_and_release('shift')
while(1):
	cv.imshow('image',img)
	k = cv.waitKey(1) & 0xFF
	if k == ord('e'):
		break

	# get current positions of four trackbars
	r = cv.getTrackbarPos('R','image')
	g = cv.getTrackbarPos('G','image')
	b = cv.getTrackbarPos('B','image')
	s = cv.getTrackbarPos(switch,'image')

	if s == 0:
		img[:] = 0
	else:
		img[:] = [b,g,r] # Pay attention to the order of 'BGR'

cv.destroyAllWindows()
```

### Core Operations

使用 `item()` 和 `itemset()` 对访问和修改单个像素具有更高的效率

```python
img = cv.imread('test.png')
print(img.item(10,10,2))
img.itemset((100,100,2),0)
print(img.item(100, 100, 2))
```

#### 分离与合并
```python
b, g, r = cv.split(img)
img = cv.merge((b, g, r))
```

#### Padding 填充
- **src** - input image
- **top**, **bottom**, **left**, **right** - border width in number of pixels in corresponding directions
- **borderType** - Flag defining what kind of border to be added. It can be following types:
    - **[cv.BORDER_CONSTANT](https://docs.opencv.org/4.x/d2/de8/group__core__array.html#gga209f2f4869e304c82d07739337eae7c5aed2e4346047e265c8c5a6d0276dcd838 "iiiiii|abcdefgh|iiiiiii with some specified i")** - Adds a constant colored border. The value should be given as next argument.
    - **[cv.BORDER_REFLECT](https://docs.opencv.org/4.x/d2/de8/group__core__array.html#gga209f2f4869e304c82d07739337eae7c5a815c8a89b7cb206dcba14d11b7560f4b "fedcba|abcdefgh|hgfedcb")** - Border will be mirror reflection of the border elements, like this : _fedcba|abcdefgh|hgfedcb_
    - **[cv.BORDER_REFLECT_101](https://docs.opencv.org/4.x/d2/de8/group__core__array.html#gga209f2f4869e304c82d07739337eae7c5ab3c5a6143d8120b95005fa7105a10bb4 "gfedcb|abcdefgh|gfedcba")** or **[cv.BORDER_DEFAULT](https://docs.opencv.org/4.x/d2/de8/group__core__array.html#gga209f2f4869e304c82d07739337eae7c5afe14c13a4ea8b8e3b3ef399013dbae01 "same as BORDER_REFLECT_101")** - Same as above, but with a slight change, like this : _gfedcb|abcdefgh|gfedcba_
    - **[cv.BORDER_REPLICATE](https://docs.opencv.org/4.x/d2/de8/group__core__array.html#gga209f2f4869e304c82d07739337eae7c5aa1de4cff95e3377d6d0cbe7569bd4e9f "aaaaaa|abcdefgh|hhhhhhh")** - Last element is replicated throughout, like this: _aaaaaa|abcdefgh|hhhhhhh_
    - **[cv.BORDER_WRAP](https://docs.opencv.org/4.x/d2/de8/group__core__array.html#gga209f2f4869e304c82d07739337eae7c5a697c1b011884a7c2bdc0e5caf7955661 "cdefgh|abcdefgh|abcdefg")** - Can't explain, it will look like this : _cdefgh|abcdefgh|abcdefg_
- **value** - Color of border if border type is [cv.BORDER_CONSTANT](https://docs.opencv.org/4.x/d2/de8/group__core__array.html#gga209f2f4869e304c82d07739337eae7c5aed2e4346047e265c8c5a6d0276dcd838 "iiiiii|abcdefgh|iiiiiii with some specified i")

```python
import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt
 
BLUE = [255,0,0]
 
img1 = cv.imread('opencv-logo.png')
assert img1 is not None, "file could not be read, check with os.path.exists()"
 
replicate = cv.copyMakeBorder(img1,10,10,10,10,cv.BORDER_REPLICATE)
reflect = cv.copyMakeBorder(img1,10,10,10,10,cv.BORDER_REFLECT)
reflect101 = cv.copyMakeBorder(img1,10,10,10,10,cv.BORDER_REFLECT_101)
wrap = cv.copyMakeBorder(img1,10,10,10,10,cv.BORDER_WRAP)
constant= cv.copyMakeBorder(img1,10,10,10,10,cv.BORDER_CONSTANT,value=BLUE)
 
plt.subplot(231),plt.imshow(img1,'gray'),plt.title('ORIGINAL')
plt.subplot(232),plt.imshow(replicate,'gray'),plt.title('REPLICATE')
plt.subplot(233),plt.imshow(reflect,'gray'),plt.title('REFLECT')
plt.subplot(234),plt.imshow(reflect101,'gray'),plt.title('REFLECT_101')
plt.subplot(235),plt.imshow(wrap,'gray'),plt.title('WRAP')
plt.subplot(236),plt.imshow(constant,'gray'),plt.title('CONSTANT')
 
plt.show()
```

#### Image Addition
`cv.add(img1, img2)`
```python
>>> x = np.uint8([250])
>>> y = np.uint8([10])
>>> print( cv.add(x,y) ) # 250+10 = 260 => 255
[[255]]
>>> print( x+y ) # 250+10 = 260 % 256 = 4
[4]
```
`cv.addWeighted(img1,alpha,img2,beta,gama)`
$$dst=\alpha\cdot img1+\beta\cdot img2+\gamma$$
```python
img1 = cv.imread('img1.png')
img2 = cv.imread('img2.png')
 
dst = cv.addWeighted(img1,0.7,img2,0.3,0)
 
cv.imshow('dst',dst)
cv.waitKey(0)
cv.destroyAllWindows()
```

### Changing Colorspaces

`cvtColor(img,TRANSFROMATION_TYPE)` 用来转换图像的色彩模式。
以下代码可打印出 opencv 所支持的所有转换 [here](color_changing_types_list.md)
```python
with open("color_changing_types_list.md", "w") as file:
	for f in dir(cv):
		if f.startswith('COLOR_'):
			file.write(f+'\n')
```

`inRange(img, pixel1, pixel2)` 可以提取出一个宽高与原图一致的掩码图片，其上数字只有 `[0,0,0]` 和 `[255,255,255]`，后者代表原图相同位置上的像素在 `pixel1` 与 `pixel2` 所围成的三维立方体之间。

通常，将 BGR 色彩模式转换成 HSV 色彩模式后，使用 `inRange()` 更易分离出我们想要的色彩区域。

#### Transformation

`resize(img,size,fx=1,fy=1,interpolation=INTER_TYPE)` 用来缩放，`INTER_AREA` 常用来缩小，`INTER_LINEAR` 与 `INTER_CUBIC` 常用来放大。

```python
res = cv.resize(img,None,fx=2, fy=2, interpolation = cv.INTER_CUBIC)
# Or
height, width = img.shape[:2]
res = cv.resize(img,(2*width, 2*height), interpolation = cv.INTER_CUBIC)
```

`warpAffine(img, two_three_matrix,size)` 用来对图片做一些仿射变换。

```python
(cols,rows) = img.shape[:2]

# 平移
shift_matrix = np.float32([[1, 0, shift_x],[0,1,shift_y]])
shift_img = cv.warpAffine(img, shift_matrix, (cols, rows))

# 旋转：需要指定旋转中心、旋转角度、和放缩系数
angle, scaler = 90, 1
rotate_matrix = cv.getRotationMatrix2D((cols/2.0,rows/2.0),angle,scaler)
rotate_img = cv.warpAffine(img, rotate_matrix, (cols, rows))

# 仿射变换：变换前后的三个点不能共线
p1,p2,p3 = [50,50],[200,50],[50,200]
q1,q2,q3 = [10,100],[200,50][100,250]
pts1 = np.float32([p1,p2,p3])
pts2 = np.float32([q1,q2,q3])
affine_matrix = cv.getAffineTransform(pts1, pts2)
affine_img = cv.warpAffine(img, affine_matrix, (cols, rows))

# 透视变换：变换前后的四个点中，任意三个点都不能共线，即这四个点必须组成四边形
p1,p2,p3,p4 = [56,65],[368,52],[28,387],[389,390]
q1,q2,q3,q4 = [0,0],[300,0],[0,300],[300,300]
size = (300, 300)
perspective_matrix = cv.getPerspectiveTransform(pts1, pts2)
perspective_img = cv.warpPerspective(img, perspective_matrix, size)

```
#### Threshold

`cv.threshold(src,thresh,maxval,type)`
```python
img = cv.imread('gradient.png', cv.IMREAD_GRAYSCALE)
assert img is not None, "file could not be read, check with os.path.exists()"
ret,thresh1 = cv.threshold(img,127,255,cv.THRESH_BINARY)
ret,thresh2 = cv.threshold(img,127,255,cv.THRESH_BINARY_INV)
ret,thresh3 = cv.threshold(img,127,255,cv.THRESH_TRUNC)
ret,thresh4 = cv.threshold(img,127,255,cv.THRESH_TOZERO)
ret,thresh5 = cv.threshold(img,127,255,cv.THRESH_TOZERO_INV)
 
titles = ['Original Image','BINARY','BINARY_INV','TRUNC','TOZERO','TOZERO_INV']
images = [img, thresh1, thresh2, thresh3, thresh4, thresh5]
 
for i in range(6):
 plt.subplot(2,3,i+1),plt.imshow(images[i],'gray',vmin=0,vmax=255)
 plt.title(titles[i])
 plt.xticks([]),plt.yticks([])
 
plt.show()
```

`cv.adaptiveThreshold(src,maxValue,adaptiveMethod,thresholdType,blockSize,C)`
`blockSize` 需要为大于 1 的奇数。
```python
nothing = lambda x:x
oimg = cv.resize(cv.imread("sudo.jpg", cv.IMREAD_GRAYSCALE), 
	(500, 500), interpolation=cv.INTER_CUBIC)

cv.namedWindow('title')
keyboard.press_and_release('shift')
cv.createTrackbar('blur','title',0,20,nothing)
cv.createTrackbar('ksize','title',0,20,nothing)
cv.createTrackbar('C','title',0,20,nothing)

while True:
	blur = cv.getTrackbarPos('blur', 'title')
	ksize = cv.getTrackbarPos('ksize', 'title')
	C = cv.getTrackbarPos('C', 'title')
	img = cv.medianBlur(oimg, blur*2+1)
	ret,th1 = cv.threshold(img, 127, 255, cv.THRESH_BINARY)
	th2 = cv.adaptiveThreshold(img, 255,\
		cv.ADAPTIVE_THRESH_MEAN_C,cv.THRESH_BINARY,ksize*2+3,C)
	th3 = cv.adaptiveThreshold(img, 255,\
		cv.ADAPTIVE_THRESH_GAUSSIAN_C,cv.THRESH_BINARY,ksize*2+3,C)
	
	cv.imshow("title", th1)

	k = cv.waitKey(1) & 0xFF
	if k == ord('e'):
		break

cv.destroyAllWindows()
```

#### Smoothing Images

`filter2D(img,ddepth,kernel,borderType=cv.BORDER_DEFAULT)`

`ddepth` 一般置为 -1 即可，kernel 的宽高需要为奇数，原图边缘默认使用了薄反射填充。
注意 `cv.BORDER_WRAP` 不被支持。
```python
kernel = np.one((5, 5),dtype=np.float32)/25.0
dst = cv.filter2d(img,-1,kernel)
```

下面一些函数都是 `fliter2D` 衍生函数，各有特殊的 `kernel`。

`blur(img,size)` `medianBlur(img,ksize)`
后者是中值滤波，边界处理不可改变， 默认为 `cv.BORDER_REPLECATE`
```python
cv.blur(img,(5,5))
cv.medianBlur(img,5) 
```
 
 `GaussianBlur(img,size,sigmaX,sigmaY=0)`
当 `sigmaY` 为 `0` 时，其会被置为 `sigmaX`。
```python
cv.GaussianBlur(img,(5,5),0)
```

`bilateralFilter(src, d, sigmaColor, sigmaSpace, borderType=cv.BORDER_DEFAULT)`

```python
cv.bilateralFilter(img,9,75,75)
```

`d` 表示在过滤过程中每个像素邻域的直径范围。如果这个值是非正数，则函数会从第五个参数sigmaSpace计算该值。
`sigmaColor`颜色空间过滤器的sigma值，越大图像越平滑。
`sigmaSpace` 如果该值较大，则意味着颜⾊相近的较远的像素将相互影响，从而使更⼤的区域中足够相似的颜色获取相同的颜色。当d>0时，d指定了邻域大小，那么不考虑sigmaSpace值，否则d正比于sigmaSpace。

相比于其它滤波器，双边滤波器的优势是在去噪声的同时较好保留边缘信息，但计算效率较低。

#### ImageGradient

`Laplacian(img,ddpeth)`
```python
cv.Laplacian(img,cv.CV_64F)
```

`Sobel(img,ddepth,dx,dy,ksize)`
```python
sobelx = cv.Sobel(img,cv.CV_64F,1,0,5)
sobely = cv.Sobel(img,cv.CV_64F,0,1,5)
```
`Scharr(img,ddepth,dx,dy)`
```python
scharr_x = cv.Scharr(img,cv.CV_64F,1,0)
scharr_y = cv.Scharr(img,cv.CV_64F,0,1)
```

#### Morphological Transformations

```python
kernel = np.ones((5,5),np.uint8) # 1的位置指定了有效像素

erosion = cv.erode(img,kernel,iterations=1)
dilation = cv.dilate(img,kernel,iterations=1)
opening = cv.morphologyEx(img,cv.MORPH_OPEN,kernel)
closing = cv.morphologyEx(img,cv.MORPH_CLOSE,kernel)
gradient = cv.morphologyEx(img,cv.MORPH_GRADIENT,kernel)
tophat = cv.morphologyEx(img,cv.MORPH_TOPHAT,kernel)
blackhat = cv.morphologyEx(img,cv.MORPH_BLACK_HAT,kernel)
```
特殊的 kernel 可以通过 `getStructuringElement(shape,size)` 获得
```python
# Rectangular Kernel
>>> cv.getStructuringElement(cv.MORPH_RECT,(5,5))
array(
[[1, 1, 1, 1, 1],
 [1, 1, 1, 1, 1],
 [1, 1, 1, 1, 1],
 [1, 1, 1, 1, 1],
 [1, 1, 1, 1, 1]], dtype=uint8)
 
# Elliptical Kernel
>>> cv.getStructuringElement(cv.MORPH_ELLIPSE,(5,5))
array(
[[0, 0, 1, 0, 0],
 [1, 1, 1, 1, 1],
 [1, 1, 1, 1, 1],
 [1, 1, 1, 1, 1],
 [0, 0, 1, 0, 0]], dtype=uint8)
 
# Cross-shaped Kernel
>>> cv.getStructuringElement(cv.MORPH_CROSS,(5,5))
array(
[[0, 0, 1, 0, 0],
 [0, 0, 1, 0, 0],
 [1, 1, 1, 1, 1],
 [0, 0, 1, 0, 0],
 [0, 0, 1, 0, 0]], dtype=uint8)
```

![[MORPH_TYPE.png]]

#### Canny Detection

`Canny(img,threshold1,threshold2)`

```python
cv.Canny(img,100,200)
```