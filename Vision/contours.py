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



def get_canny_edge(pic):
    img = pic
    
    # Blur the image for better edge detection
    img_blur = cv2.GaussianBlur(img, (3,3), 0)

    # kernel = np.array([[0, -1, 0],
    #                [-1, 5,-1],
    #                [0, -1, 0]])
    # image_sharp = cv2.filter2D(src=img, ddepth=-1, kernel=kernel)

    # Canny Edge Detection
    edges = cv2.Canny(image=img_blur, threshold1=100, threshold2=200)

    return edges
