import cv2


def red_mask(img, lower=[(0,80,100), (5,255,255)], higher=[(165,80,100), (180,255,255)]):
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mask1 = cv2.inRange(img_hsv, lower[0], lower[1])
    mask2 = cv2.inRange(img_hsv, higher[0], higher[1])
    mask = cv2.bitwise_or(mask1, mask2 )
    croped = cv2.bitwise_and(img, img, mask=mask)
    return croped


def blue_mask(img, lower=[(80,50,180)], higher=[(150, 255, 255)]):
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(img_hsv, lower[0], higher[0])
    croped = cv2.bitwise_and(img, img, mask=mask)
    return croped


def white_mask(img, lower=[(0,0,220)], higher=[(120, 120, 255)]):
    img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(img_hsv, lower[0], higher[0])
    croped = cv2.bitwise_and(img, img, mask=mask)
    return croped
