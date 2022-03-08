import cv2
import threading
import numpy as np
import Vision.calibrate as calibrate
import Vision.contours as contours
import Vision.poly as pl
import Vision.mask as mask
import Vision.cubes as cubes
import Vision.arucodetect as ad

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
    else: return _frame


def run():
    i = 0
    cubeorientation = 0
    cube_coo = ()
    num_frame = 0
    color_list = []
    while(True):
        if cv2.waitKey(1) == 27:
            break
        if cv2.waitKey(1) == 32:
            imagenumber += 1
            name = str(imagenumber)
            cv2.imwrite('imaged%s.png'%name, calibrate.undistort_fisheye(_frame))

        ########## image set up and contour
        current_frame = calibrate.undistort_fisheye(get_frame())
        current_frame_isblue = current_frame.copy()[580:759, 50:250]
        current_frame_copy = current_frame.copy()
        blank_img = np.zeros(shape=current_frame.shape, dtype=np.uint8)
        current_contour = contours.get_contour(current_frame)
        cv2.drawContours(current_frame, current_contour, -1, (0, 255, 0), thickness=1)
        
        ########## cube detection red mask
        # current_frame_left_red = mask.red_mask(current_frame[580:759, 50:250])   ###### y axis from top, x axis from left
        current_left_red = mask.white_mask(current_frame[580:759, 50:250],lower=[(122,52,91)], higher=[(255, 248, 253)])
        cv2.imshow('left',current_left_red)
        current_left_contours = contours.get_contour(current_left_red)

        ######### draws special points
        cv2.line(current_frame,(683,197),(315,533),(255,0,0),2)
        current_frame = pl.draw_points_yx(img = current_frame, coordinate_list=[(55,618),(128,535),(270,833),(349,749)], color = (255, 0, 0))
        
        ######### gets and draws cubes on current_frame
        current_left,_,__ = cubes.getrectbox(current_frame_copy[580:759, 50:250],current_left_contours)
        cube, frame = cubes.getcube(current_frame_copy, current_left_contours, 50, 580)
        # if cube:
        #     cube_center = [int(cube[0][1]),int(cube[0][0])]
        #     current_frame = pl.draw_line_rot(frame, cube_center, 90-cube[2])

        # ######## gets and draws robot aruco detection on current_frame
        #     current_frame, corners, edge_center, centerline_rotation = ad.detectaruco(current_frame,font)
        #     if corners:
        #         current_frame = pl.draw_line_rot(current_frame,edge_center,centerline_rotation)

        ########## image resize
        # current_frame = cv2.resize(current_frame, tuple([int(1.5 * current_frame.shape[1]), int(1.5 * current_frame.shape[0])]))
        
        ############# image output
        iblue, pic = cubes.isBlue(current_frame_isblue)
        cv2.imshow("frame", pic)
        
        print(iblue)
        if cv2.waitKey(1) == 27:
            break
        
    _capture.release()
