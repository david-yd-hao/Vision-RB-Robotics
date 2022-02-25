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
    blank_img_copy = blank_img.copy()
    rects_list, box_list = getrectbox(blank_img_copy, contours)
    cubes_list = []
    cubes_list.append(rects_list[0])

    for i in range(1,len(rects_list)):
        duplicated_state = False
        centerx = rects_list[i][0][0]
        centery = rects_list[i][0][1]
        width = rects_list[i][1][0]
        height = rects_list[i][1][1]
        orientation = rects_list[i][1]

        for j in range(0,len(cubes_list)):
            distance = np.sqrt((centerx - cubes_list[j][0][0])**2 + (centery - cubes_list[j][0][1])**2) ####### distance of centers
            if distance > 3:
                continue
            if distance < 3:
                duplicated_state = True
                cubes_list[j][0][0] = (centerx + cubes_list[j][0][0])/2 ######## average center on x axis
                cubes_list[j][0][1] = (centery + cubes_list[j][0][1])/2  ######## average center on y axis
                cubes_list[j][1][0] = (width + cubes_list[j][1][0])/2  ######## average width
                cubes_list[j][1][1] = (height + cubes_list[j][1][1])/2 ######## average height
                cubes_list[j][2] = (orientation + cubes_list[j][2])/2  ######## average orientation
                break

        if duplicated_state == False:
            cubes_list.append(rects_list[i])

    return cubes_list




