import cv2
import numpy as np
import poly as pl


##### rects_list is a 2d list (rects are (center point, rotation), box_list is 3d list
def getrectbox(blank_img, contours):

    blank_img_copy = blank_img.copy()
    rects_list = []
    box_list = []
    
    for i in range(0,len(contours)):
        rect_para = []
        rect = cv2.minAreaRect(contours[i])

        ##### skip rect too large or too small
        area = rect[1][0] * rect[1][1]
        
        if(area > 150 and area < 500):
            rect_para.append([rect[0][0],rect[0][1]]) ####### center
            rect_para.append([rect[1][0],rect[1][1]]) #######  height and width
            rect_para.append(rect[2]) ####### orientation
            rects_list.append(rect_para)
            box = cv2.boxPoints(rect)
            box = np.int0(box)
            cv2.drawContours(blank_img_copy,[box],0,(0,0,255),2)
            box_list.append(box)

    return blank_img_copy, rects_list, box_list


###### get the cubes in cube zone 
###### added coordinate correction
def getcube(blank_img, contours, x_lim, y_lim):

    #### add division by zeros

    blank_img_copy = blank_img.copy()
    img_cache, rects_list, box_list = getrectbox(blank_img, contours)
    if rects_list:
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
        
        centerx = int(centerx / num)
        centery = int(centery / num)
        width = width / num
        height = height / num
        orientation = orientation / num

        cube = [[centery + y_lim, centerx + x_lim] , [width,height],orientation]
        blank_img_copy = pl.draw_points_yx(blank_img_copy, [[centery + y_lim, centerx + x_lim]])
        return cube, blank_img_copy
    else:
        return None, blank_img_copy

