import cv2
import threading
import numpy as np
import calibrate
import contours
import poly as pl
import mask
import cubes
import cv2.aruco as aruco

########## select camera
_capture = cv2.VideoCapture("http://localhost:8081/stream/video.mjpeg")
_capture.set(cv2.CAP_PROP_BUFFERSIZE, 0)
imagenumber = 0


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


if __name__ == "__main__":
    i = 0
    while(True):
        if cv2.waitKey(1) == 27:
            break
        if cv2.waitKey(1) == 32:
            imagenumber += 1
            name = str(imagenumber)
            cv2.imwrite('imaged%s.png'%name, calibrate.undistort_fisheye(_frame))

        ########## image set up
        current_frame = calibrate.undistort_fisheye(get_frame())
        current_frame_blank = current_frame.copy()
        current_frame_green = mask.green_mask(current_frame.copy(), lower = [(50, 5, 80)], higher = [(90, 255, 245)])
        current_frame_left_red = mask.red_mask(current_frame[500:759, 0:400])   ###### y axis from top, x axis from left
        cv2.imshow('left',current_frame_left_red)
        blank_img = np.zeros(shape=current_frame.shape, dtype=np.uint8)

        ######### gets and draws contours on original & left bottom
        current_contour = contours.get_contour(current_frame)
        current_left_contours = contours.get_contour(current_frame_left_red)
        cv2.drawContours(current_frame, current_contour, -1, (0, 255, 0), thickness=1)
        ######### draws special points
        cv2.line(current_frame,(683,197),(315,533),(255,0,0),2)
        current_frame = pl.draw_blank(blank = current_frame, coordinate_list=[(55,618),(128,535),(270,833),(349,749)], color = (255, 0, 0))
        ######### gets and draws cubes on blank
        cube, blank_img_cube = cubes.getcube(blank_img, current_left_contours, 0, 500)

        ######## gets aruco detection

        
        ########## gets and draws qr code position and angle

        
        
        ########## image resize
        # current_frame = cv2.resize(current_frame, tuple([int(1.5 * current_frame.shape[1]), int(1.5 * current_frame.shape[0])]))
        ############# image output
        cv2.imshow("frame", current_frame)
        cv2.imshow('cube', blank_img_cube)

        if cv2.waitKey(1) == 27:
            break
        
    _capture.release()
