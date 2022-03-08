import cv2
import numpy as np
import poly as pl
import mask
import contours
import calibrate

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
        
        if num == 1:
            centerx =rects_list[0][0][0]
            centery = rects_list[0][0][1]
            width = rects_list[0][1][0]
            height = rects_list[0][1][1]
            orientation = rects_list[0][2]
        elif num == 2:
            centerx = int((rects_list[0][0][0] + rects_list[1][0][0])/2)
            centery = int((rects_list[0][0][1] + rects_list[1][0][1])/2)
            width = (rects_list[0][1][0] + rects_list[1][1][0])/2
            height = (rects_list[0][1][1] + rects_list[1][1][1])/2
            a = rects_list[0][2]
            b = rects_list[1][2]
            if a == 90:
                a = 0
            if b ==90:
                b = 0
            print(a,b)
            if abs(a-b)<60:
                orientation = (a+b)/2
            elif abs(a-b)>60:
                orientation = (a + b - 90)/2
            print(orientation)


        cube = [[centery + y_lim, centerx + x_lim] , [width,height],orientation]
        blank_img_copy = pl.draw_points_yx(blank_img_copy, [[centery + y_lim, centerx + x_lim]])
        return cube, blank_img_copy
    else:
        return None, blank_img_copy

capture = cv2.VideoCapture("http://localhost:8081/stream/video.mjpeg")
x1 = 700
x2 = 1016
y1 = 0
y2 = 150

if __name__ == "__main__":

    while True:
        _rval, frame = capture.read()
        # frame = cv2.imread('/Users/davidhao/GitProject/CamProject/Vision-RB-Robotics/Vision/TestPics/cube/UDT_imaged11.png')
        frame = calibrate.undistort_fisheye(frame)
        current_left_red = mask.white_mask(frame[y1:y2, x1:x2],lower=[(154,48,104)], higher=[(208, 217, 255)])
        cv2.imshow('mask',current_left_red)
        current_left_contours = contours.get_contour(current_left_red)
        current_left,_,__ = getrectbox(frame[y1:y2, x1:x2],current_left_contours)
        cube, frame = getcube(frame, current_left_contours, x1, y1)
        if cube:
            cube_center = [int(cube[0][1]),int(cube[0][0])]
            print(cube_center)
            print(cube[2])
            frame = pl.draw_line_rot(frame, cube_center, 90-cube[2])

        cv2.imshow("frame",frame)
        cv2.imshow('red', current_left)
        if cv2.waitKey(1) == 27:
            break