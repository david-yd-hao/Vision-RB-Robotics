from calendar import c
import cv2
import threading
import calibrate
import contours
import numpy as np
import poly as pl
import corners
import mask



K2 = np.array([[637.8931714029114, 0.0, 509.67125143385334], [0.0, 636.4000140079311, 371.2613659540199], [0.0, 0.0, 1.0]])
D2 = np.array([[-0.02628723220492124], [-0.1740869162806197], [0.11587794888959864], [0.041124156040405195]])
DIM2 = (1016, 760)        


img = cv2.imread("/Users/davidhao/GitProject/CamProject/Vision-RB-Robotics/TestPics/DT_imaged5.png")
img = calibrate.undistort_fisheye(img, K2, D2, DIM2)
########## image set up
     
current_frame = img

current_frame = current_frame[600:759, 0:300]
blank_image = np.zeros(shape=current_frame.shape, dtype=np.uint8)

current_frame = mask.red_mask(current_frame)

######### gets and draws boundary of contours. 
current_contour = contours.get_contour(current_frame)
cv2.drawContours(current_frame, current_contour, -1, (0, 255, 0), thickness=1)

########### get and draws corner dots and center dots
plist, p_list, current_frame = corners.draw_corners(current_frame)
polygons = pl.get_poly(current_frame)
average = pl.get_average(polygons)
current_frame = corners.draw_points(current_frame, average, 2, (0, 255, 0))
# blank_image = corners.draw_points(blank_image, average, 2, (0, 255, 0))

########## image resize
current_frame = cv2.resize(current_frame, tuple([int(1.5 * current_frame.shape[1]), int(1.5 * current_frame.shape[0])]))
blank_copy = blank_image.copy()
############# image output
cv2.imshow("frame", current_frame)
cv2.imshow('dots', pl.draw_blank(blank_image, p_list[0], (255,0,0)))
# cv2.imshow('dots2', pl.draw_blank(blank_copy, p_list[1], (0,255,0)))
# cv2.imshow('lefts', corners.draw_points(corners.draw_points(blank_image, left_list, 1, (255, 0, 0)), bottom_list, 1, (0, 255, 0)))

print(len(current_contour))

for i in range(0,len(current_contour)):
    rect=cv2.minAreaRect(current_contour[i])
    box = cv2.boxPoints(rect)
    box = np.int0(box)
    cv2.drawContours(blank_copy,[box],0,(0,0,255),2)

cv2.imshow('rect',blank_copy)




cv2.waitKey(0)
