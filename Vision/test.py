import cv2
import corners
import calibrate
import numpy as np
import poly as pl
import pandas as pd
def get_average(polygons):
    average_points = []
    for i in polygons:
        sum_x = 0
        sum_y = 0
        count = 0
        for j in i:
            sum_x += j[0][0]
            sum_y += j[0][1]
            count += 1
        average_points.append([int(sum_x/count), int(sum_y/count)])
    return average_points
K2 = np.array([[637.8931714029114, 0.0, 509.67125143385334], [0.0, 636.4000140079311, 371.2613659540199], [0.0, 0.0, 1.0]])
D2 = np.array([[-0.02628723220492124], [-0.1740869162806197], [0.11587794888959864], [0.041124156040405195]])
DIM2 = (1016, 760)
img = cv2.imread("C:\\Users\\Shuohan\\Desktop\\ML\\IDP\\TestPics\\DT_imaged1.png")
img = calibrate.undistort_fisheye(img, K2, D2, DIM2)
polygons = pl.get_poly(img)
average = get_average(polygons)
plist, current_frame = corners.draw_corners(img)
cv2.imshow('frame', current_frame)
blank_image = np.zeros(shape=current_frame.shape, dtype=np.uint8)
cv2.imshow('averages', corners.draw_points(current_frame, average, 3, (0, 255, 0)))
cv2.waitKey(0)