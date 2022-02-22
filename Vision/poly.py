import cv2
import numpy as np
import contours

# Read the original image
#img = cv2.imread('/Users/davidhao/GitProject/CamProject/Vision-RB-Robotics/Pics/tablecube_UDT1.png')

def get_poly(pic):
    img = pic

    font = cv2.FONT_HERSHEY_COMPLEX

    contour = contours.get_contour(img)
    polygons_list = []
    for cnt in contour : 

        approx = cv2.approxPolyDP(cnt, 0.001 * cv2.arcLength(cnt, True), True) 
        polygons_list.append(approx)

    return polygons_list

def draw_blank(blank, coordinate_list):
    for i in coordinate_list:
        x = i[0]
        y = i[1]
        blank = cv2.circle(blank, (x,y), radius=0, color=(255, 255, 255), thickness=-1) 
    return blank
# Showing the final image. 
#cv2.imshow('image2', img)
#cv2.waitKey(0)


# print(contour)