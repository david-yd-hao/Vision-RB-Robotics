import numpy as np
import cv2


def get_contour(pic):
    im = pic
    # Convert to graycsale
    imgray = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
    imgray = get_canny_edge(imgray)
    ret, thresh = cv2.threshold(imgray, 230, 255, 0)
    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    return contours



def get_sobel_edge(pic):
    img = pic
    img_blur = cv2.GaussianBlur(img, (3,3), sigmaX=0.1, sigmaY=0.1)
    grad_x = cv2.Sobel(src=img_blur, ddepth=cv2.CV_16S, dx=1, dy=0, ksize=3)
    grad_y = cv2.Sobel(src=img_blur, ddepth=cv2.CV_16S, dx=0, dy=1, ksize=3)
    abs_grad_x = cv2.convertScaleAbs(grad_x)
    abs_grad_y = cv2.convertScaleAbs(grad_y)
    return abs_grad_x + abs_grad_y



def get_canny_edge(pic):
    img = pic
    
    ######### Blur the image for better edge detection
    img = cv2.GaussianBlur(img, (3,3), 0)


    ######## Sharpen the image
    # kernel = np.array([[0, -1, 0],
    #                [-1, 5,-1],
    #                [0, -1, 0]])
    # image_sharp = cv2.filter2D(src=img, ddepth=-1, kernel=kernel)

    ######### Canny Edge Detection
    edges = cv2.Canny(image=img, threshold1=60, threshold2=200)

    return edges
