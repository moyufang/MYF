import cv2 as cv
import sys
import numpy as np
import keyboard
from matplotlib import pyplot as plt
 
def test1():
	img1 = cv.imread(r'.\test.png', cv.IMREAD_COLOR)
	img2 = cv.imread(r'.\test.png', cv.IMREAD_UNCHANGED)
	img3 = cv.imread(r'.\test.png', cv.IMREAD_GRAYSCALE)
	print(type(img1))
	print(img1.shape, img2.shape, img3.shape)
	cv.namedWindow('Title')
	keyboard.press_and_release('shift')
	
	cv.imshow("Title", img1); k = cv.waitKey(0)
	cv.imshow("Title", img2); k = cv.waitKey(0)
	cv.imshow("Title", img3); k = cv.waitKey(0)
	 
	#  cv.imwrite("starry_night.png", img)

def test2():
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

drawing = False # true if mouse is pressed
mode = True # if True, draw rectangle. Press 'm' to toggle to curve
ix,iy = -1,-1
 
# mouse callback function
def draw_circle(event,x,y,flags,param):
	global ix,iy,drawing,mode,img
	radius = 20
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

def test3():
	global img,mode
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


def test4():
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
			img[:] = [b,g,r]

	cv.destroyAllWindows()

def test5(): 
	BLUE = [255,0,0]
	 
	img1 = cv.imread('test.png')
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

def test6():
	img1 = cv.imread(r'Utena.png', cv.IMREAD_COLOR)
	img2 = cv.imread(r'QQPlayerIcon.png', cv.IMREAD_COLOR)
	print(img1.shape, img2.shape)

	rows, cols, channels = img2.shape
	roi = img1[0:rows, 0:cols]

	img2gray = cv.cvtColor(img2, cv.COLOR_BGR2GRAY)
	ret, mask = cv.threshold(img2gray, 50, 255, cv.THRESH_BINARY)
	mask_inv = cv.bitwise_not(mask)

	img1_bg = cv.bitwise_and(roi,roi,mask = mask_inv)
	img2_fg = cv.bitwise_and(img2,img2,mask = mask)

	dst = cv.add(img1_bg,img2_fg)
	img1[0:rows, 0:cols ] = dst

	print("img2gray:", type(img2gray), img2gray.dtype)
	print("ret:", type(ret), ret)
	print("mask:", type(mask), mask.dtype, mask.item(0,0))
	print("mask_inv:", type(mask_inv), mask_inv.dtype, mask_inv.item(0,0))
	print("img1_bg:", type(img1_bg), img1_bg.dtype)
	print("img2_fg:", type(img2_fg), img2_fg.dtype)

	plt.subplot(231),plt.imshow(cv.cvtColor(img2gray, cv.COLOR_BGR2RGB)),plt.title('img2gray')
	plt.subplot(232),plt.imshow(cv.cvtColor(mask, cv.COLOR_BGR2RGB)),plt.title('mask')
	plt.subplot(233),plt.imshow(cv.cvtColor(mask_inv, cv.COLOR_BGR2RGB)),plt.title('mask_inv')
	plt.subplot(234),plt.imshow(cv.cvtColor(img2, cv.COLOR_BGR2RGB)),plt.title('img2')
	plt.subplot(235),plt.imshow(cv.cvtColor(img1_bg, cv.COLOR_BGR2RGB)),plt.title('img1_bg')
	plt.subplot(236),plt.imshow(cv.cvtColor(img2_fg, cv.COLOR_BGR2RGB)),plt.title('img2_fg')

	plt.show()

	return

def test7():
	img = cv.imread('gradient.png', cv.IMREAD_GRAYSCALE)

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

def test8():
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
		th2 = cv.adaptiveThreshold(img, 255, cv.ADAPTIVE_THRESH_MEAN_C,cv.THRESH_BINARY,ksize*2+3,C)
		th3 = cv.adaptiveThreshold(img, 255, cv.ADAPTIVE_THRESH_GAUSSIAN_C,cv.THRESH_BINARY,ksize*2+3,C)
		
		cv.imshow("title", th1)

		k = cv.waitKey(1) & 0xFF
		if k == ord('e'):
			break

	cv.destroyAllWindows()

def test9():
	nothing = lambda x:x
	oimg = cv.resize(cv.imread("sudo.jpg", cv.IMREAD_GRAYSCALE),
		(500,500), interpolation=cv.INTER_LINEAR)
	
	cv.namedWindow('title')
	keyboard.press_and_release('shift')
	cv.createTrackbar('d','title',0,20,nothing)
	cv.createTrackbar('color','title',0,20,nothing)
	cv.createTrackbar('space','title',0,20,nothing)
	cv.createTrackbar('ksize','title',0,15,nothing)
	cv.createTrackbar('sigma','title',0,20,nothing)

	while True:
		d = cv.getTrackbarPos('d', 'title')
		color = cv.getTrackbarPos('color', 'title')
		space = cv.getTrackbarPos('space', 'title')
		ksize = cv.getTrackbarPos('ksize', 'title')
		sigma = cv.getTrackbarPos('sigma', 'title')

		img = cv.bilateralFilter(oimg, d, color, space)
		gaussian = cv.GaussianBlur(img,(ksize*2+1,ksize*2+1),sigma)
		laplacian = cv.Laplacian(img, cv.CV_64F)
		sobelx = cv.Sobel(img, cv.CV_64F, 1, 0, ksize=2*ksize+1)
		sobely = cv.Sobel(img, cv.CV_64F, 0, 1, ksize=2*ksize+1)
		scharr = cv.Scharr(img, cv.CV_64F, 0, 1)
		cv.imshow("title", laplacian)

		k = cv.waitKey(1) & 0xFF
		if k == ord('e'):
			break

	cv.destroyAllWindows()

#test9()

for f in dir(cv):
	if f.startswith('COLOR_'):
		print(f)