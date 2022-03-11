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
from turtle import delay
from time import sleep
########## select camera
_capture = cv2.VideoCapture("http://localhost:8081/stream/video.mjpeg")
_capture.set(cv2.CAP_PROP_BUFFERSIZE, 0)
imagenumber = 0
font = cv2.FONT_HERSHEY_SIMPLEX


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


def run():

    i = 0
    cubeorientation = 0
    cube_coo = ()
    num_frame = 0
    color_list = []
    isBluelist = []
    bluefinal = False
    start = False
    while(True):
        if cv2.waitKey(1) == 27:
            break
        if cv2.waitKey(1) == 32:
            imagenumber += 1
            name = str(imagenumber)
            cv2.imwrite('imaged%s.png'%name, calibrate.undistort_fisheye(_frame))
        i += 1
        ########## image set up and contour
        current_frame = calibrate.undistort_fisheye(get_frame())
        print(current_frame.shape)
        cv2.imshow("left", current_frame[597:, 0:210])
        cv2.imshow("cam2", current_frame)
        if i<= 30:
            current_frame_isblue = current_frame.copy()[597:, 0:210]
            iblue, pic = cubes.isBlue(current_frame_isblue)
            isBluelist.append(iblue)
        if i == 31:
            blue = 0
            red = 0
            for i in isBluelist:
                if not i:
                    red += 1
                if i:
                    blue += 1
            if 2 * blue >= red:
                bluefinal = True
            else:
                bluefinal = False
            start = True
        cv2.imshow("ori", current_frame_isblue)
        current_frame_copy = current_frame.copy()
        blank_img = np.zeros(shape=current_frame.shape, dtype=np.uint8)
        current_contour = contours.get_contour(current_frame)
        cv2.drawContours(current_frame, current_contour, -1, (0, 255, 0), thickness=1)
        
        ########## cube detection red mask
        # current_frame_left_red = mask.red_mask(current_frame[580:759, 50:250])   ###### y axis from top, x axis from left
        current_left_red = mask.white_mask(current_frame[597:, 0:210],lower=[(122,52,91)], higher=[(255, 248, 253)])
        cv2.imshow('left',current_left_red)
        current_left_contours = contours.get_contour(current_left_red)

        ######### draws special points
        cv2.line(current_frame,(263,540),(591,197),(255,0,0),2)
        
        ######### gets and draws cubes on current_frame
        current_left,_,__ = cubes.getrectbox(current_frame_copy[597:, 0:210],current_left_contours)
        cube, frame = cubes.getcube(current_frame_copy, current_left_contours, 50, 580)
        if cube:
            cube_center = [int(cube[0][1]),int(cube[0][0])]


        ######## gets and draws robot aruco detection on current_frame
        current_frame, corners, edge_center, centerline_rotation = ad.detectaruco(current_frame,font)
        if corners:
            current_frame = pl.draw_line_rot(current_frame,edge_center,centerline_rotation)

        ######### image resize
        current_frame = cv2.resize(current_frame, tuple([int(1.5 * current_frame.shape[1]), int(1.5 * current_frame.shape[0])]))
        
        ############ image output
        
        cv2.imshow("frame", current_frame)
        # cv2.imshow("whole", current_frame)
        start = int(start)
        bluefinal = int(bluefinal)
        com.send_error(pic, pic, pic, pic, pic, pic, pic, pic, pic, centerline_rotation, bluefinal, start)
        if cv2.waitKey(1) == 27:
            break
        #sleep(0.05)
    _capture.release()


if __name__ == "__main__":
    run()
