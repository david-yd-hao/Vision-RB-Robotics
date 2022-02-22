import numpy as np
import cv2

def get_contour(pic):
    im = pic
    # Convert to graycsale
    imgray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
    imgray = get_sobel_edge(imgray)
    ret, thresh = cv2.threshold(imgray, 127, 255, 0)
    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    return contours


def get_canny_edge(pic):
    img = pic
    
    # Blur the image for better edge detection
    img_blur = cv2.GaussianBlur(img, (3,3), 0)

    # Canny Edge Detection
    edges = cv2.Canny(image=img_blur, threshold1=100, threshold2=200)

    return edges


def get_sobel_edge(pic):
    img = pic

    # Blur the image for better edge detection
    img_blur = cv2.GaussianBlur(img, (3,3), 0)
    
    # Sobel Edge Detection
    sobelxy = cv2.Sobel(src=img_blur, ddepth=cv2.CV_64F, dx=1, dy=1, ksize=5) # Combined X and Y Sobel Edge Detection

    return sobelxy