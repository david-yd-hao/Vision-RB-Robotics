import cv2
import threading
import numpy as np
import calibrate as calibrate
import contours as contours
import poly as pl
import mask as mask
import cubes as cubes
import arucodetect as ad
import communicate as com
from time import sleep
from datetime import datetime
from joblib import load
from cluster import predictSV
########## select camera
_capture = cv2.VideoCapture("http://localhost:8081/stream/video.mjpeg")
_capture.set(cv2.CAP_PROP_BUFFERSIZE, 0)
imagenumber = 0
font = cv2.FONT_HERSHEY_SIMPLEX
buffer_time = 0.03
if _capture.isOpened(): 
    _rval, _frame  = _capture.read()
else:
    _rval = False
_grey = None
def _reader_func():
    global _frame, _grey
    while True:
        _rval, _f = _capture.read()
        if _rval:
            _frame = _f
            _grey = cv2.cvtColor(_f, cv2.COLOR_BGR2GRAY)
_reader = threading.Thread(target=_reader_func)
_reader.daemon = True
_reader.start()
def get_frame(copy_=True):
    global _frame
    if copy_: return _frame.copy()
    else: return 


########## Main Function
def run():
    filename = "svmodel.joblib"
    sv = load(filename)
    bluefinal = False
    start = False
    isBlue = 0

    shot = False
    while(True):


        cube_center_x = 0
        cube_center_y = 0
        robotedge_center_x = 0
        robotedge_center_y = 0

        ########## image set up
        current_frame = calibrate.undistort_fisheye(get_frame())
        if shot == False:
            imgblue = current_frame
            isBlue = predictSV(sv, imgblue)[0]
            shot = True
            print(isBlue)

        current_frame_copy = current_frame.copy()

       
        ########## cube detection mask contours
        current_left_red = mask.white_mask(img=current_frame[597:, 30:210], lower=[(121, 58, 144)], higher=[(255, 194, 251)])
        cv2.imshow('left',current_left_red)
        current_left_contours = contours.get_contour(current_left_red)
        
        ######### gets and draws cubes on current_frame
        cube, current_frame = cubes.getcube(current_frame, current_left_contours, 30, 597)
        if cube:
            cube_center_x = int(cube[0][1])
            cube_center_y = int(cube[0][0])

        ######## gets and draws robot aruco detection on current_frame
        current_frame, corners, edge_center, robot_rotation = ad.detectaruco(current_frame,font)
        if corners:
            robotedge_center_x = int(edge_center[0])
            robotedge_center_y = int(edge_center[1])
            current_frame = pl.draw_line_rot(current_frame,edge_center,robot_rotation)

        ######### image resize
        current_frame = cv2.resize(current_frame, tuple([int(1.5 * current_frame.shape[1]), int(1.5 * current_frame.shape[0])]))
        
        ############ image output
        cv2.imshow("frame", current_frame)

        ############ communication with robot
        start = int(start)

        ###### buffer communication
        # t2 = datetime.now().timestamp()
        # while(t2 - t_before <= buffer_time):
        #     sleep(0.001)
        #     t2 = datetime.now().timestamp()
        com.send_error("10.254.223.22", robotedge_center_x, robotedge_center_y, int(robot_rotation), cube_center_x, cube_center_y, int(isBlue), start)
        t_before = datetime.now().timestamp()
        sleep(0.003)

        ########### Break Key and Wait for 0.05 sec
        if cv2.waitKey(1) == 27:
            break
    _capture.release()


if __name__ == "__main__":
    run()
