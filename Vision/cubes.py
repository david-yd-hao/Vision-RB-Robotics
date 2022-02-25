import cv2
import numpy as np


##### rects_list is a 2d list (rects are (center point, rotation), box_list is 3d list
def getrectbox(blank_img, contours):

    rects_list = []
    box_list = []
    
    for i in range(0,len(contours)):
        rect_para = []
        rect = cv2.minAreaRect(contours[i])

        ##### skip rect too large or too small
        if (rect[1][0] * rect[1][1]) > 500:
            continue
        if (rect[1][0] * rect[1][1]) < 150:
            continue

        rect_para.append([rect[0][0],rect[0][1]]) ####### center
        rect_para.append([rect[1][0],rect[1][1]]) #######  height and width
        rect_para.append(rect[2]) ####### orientation
        rects_list.append(rect_para)
        box = cv2.boxPoints(rect)
        box = np.int0(box)
        cv2.drawContours(blank_img,[box],0,(0,0,255),2)
        box_list.append(box)

    return blank_img, rects_list, box_list


def getcube(blank_img, contours):

    #### add division by zero

    blank_img_copy = blank_img.copy()
    img, rects_list, box_list = getrectbox(blank_img_copy, contours)
    cube = []
    
    num = len(rects_list)
    centerx = 0
    centery = 0
    width = 0
    height = 0
    orientation = 0

    for i in range(0, num):
        centerx = centerx + rects_list[i][0][0]
        centery = centery + rects_list[i][0][1]
        width = width + rects_list[i][1][0]
        height = height + rects_list[i][1][1]
        orientation = orientation + rects_list[i][2]
    
    centerx = centerx / num
    centery = centery / num
    width = width / num
    height = height / num
    orientation = orientation / num

    cube = [[centerx, centery], [width,height],orientation]
    return cube




