
import cv2
import numpy as np
from ipywidgets import *
import matplotlib.pyplot as plt
import ipython_genutils as ip
def sharpen(img):
    kernel = np.array([[0, -1, 0],
                   [-1, 5,-1],
                   [0, -1, 0]])
    img = cv2.filter2D(src=img, ddepth = -1, kernel = kernel)
    return img
def undistort_fisheye(img, K=np.array([[637.8931714029114, 0.0, 509.67125143385334], [0.0, 636.4000140079311, 371.2613659540199], [0.0, 0.0, 1.0]]), D=np.array([[-0.02628723220492124], [-0.1740869162806197], [0.11587794888959864], [0.041124156040405195]]), DIM=(1016, 760)):
    h,w = img.shape[:2]
    cam = np.eye(3)
    map1, map2 = cv2.fisheye.initUndistortRectifyMap(K, D, cam, K, DIM, cv2.CV_16SC2)
    undistorted_img = cv2.remap(img, map1, map2, interpolation=cv2.INTER_LINEAR, borderMode=cv2.BORDER_DEFAULT)
    return undistorted_img
def f(hMin ,hMax ,Smin,Smax ,Vmin,Vmax):
    img1 = cv2.imread('DT_imaged19.png')
    img1 = undistort_fisheye(img1)

    img1 = img1[500:759, 0:400]
    img = cv2.cvtColor(img1 ,cv2.COLOR_BGR2RGB)  #in matplot image show in rgb format in cv2 it show in bgr format remeber this else you will face problem
    hsv = cv2.cvtColor(img,cv2.COLOR_RGB2HSV)# if you are going to use cv2 to show image then cv2.COLOR_BGR2HSV 
    Lhsv = np.array([hMin,Smin,Vmin])
    Uhsv = np.array([hMax ,Smax,Vmax])
    mask = cv2.inRange(hsv,Lhsv ,Uhsv)
    r = cv2.bitwise_and(img,img,mask =mask)
    
    
    plt.imshow(r)

    
interactive_plot = interactive(f,hMin=(0,255),hMax = (0,255),Smin = (0,255),Smax =(0,255),Vmin=(0,255),Vmax=(0,255))
interactive_plot