import cv2
import numpy as np

def red_mask(img, lower=[(0,80,100), (5,255,255)], higher=[(165,80,100), (180,255,255)]):
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mask1 = cv2.inRange(img_hsv, lower[0], lower[1])
    mask2 = cv2.inRange(img_hsv, higher[0], higher[1])
    mask = cv2.bitwise_or(mask1, mask2 )
    croped = cv2.bitwise_and(img, img, mask=mask)
    return croped


def blue_mask(img, lower=[(100,80,90)], higher=[(140, 120, 155)]):
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(img_hsv, lower[0], higher[0])
    croped = cv2.bitwise_and(img, img, mask=mask)
    return croped


def white_mask(img, lower=[(0,0,220)], higher=[(120, 120, 255)]):
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(img_hsv, lower[0], higher[0])
    croped = cv2.bitwise_and(img, img, mask=mask)
    return croped

def green_mask(img, lower = [(140, 20, 60)], higher = [(180, 60, 100)]):
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(img_hsv, lower[0], higher[0])
    croped = cv2.bitwise_and(img, img, mask=mask)
    return croped

def anti_glare(img, lower = [(0, 0, 0)], higher = [(255, 127, 250)]):
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(img_hsv, lower[0], higher[0])
    croped = cv2.bitwise_and(img, img, mask=mask)
    return croped

def sharpen(img):
    kernel = np.array([[0, -1, 0],
                   [-1, 5,-1],
                   [0, -1, 0]])
    img = cv2.filter2D(src=img, ddepth = -1, kernel = kernel)
    return img
