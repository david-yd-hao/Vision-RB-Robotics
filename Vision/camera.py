import cv2
import threading
import numpy as np
import calibrate
import contours
import poly as pl
import mask
import cubes
import qrdetect as qr
import testqr as tq

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
    avg_points_over_time = []
    i = 0
    calibrate_frame_num = 31
    red_destinations = []
    while(True):
        if cv2.waitKey(1) == 27:
            break
        if cv2.waitKey(1) == 32:
            imagenumber += 1
            name = str(imagenumber)
            cv2.imwrite('imaged%s.png'%name, calibrate.undistort_fisheye(_frame))

        ########## image set up
        current_frame = calibrate.undistort_fisheye(get_frame())
        #current_frame_denoise = cv2.fastNlMeansDenoisingColored(current_frame, None, 3, 10, 7, 21)
        #cv2.imshow("denoised", current_frame_denoise)
        current_frame_blank = current_frame.copy()
        current_frame_green = current_frame.copy()
        current_frame_b = current_frame.copy()
        current_frame_glare = current_frame.copy()
        current_frame_green = mask.green_mask(current_frame_green, lower = [(50, 10, 100)], higher = [(90, 255, 245)])
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

        current_frame_green = tq.qrdet(current_frame_green)
        ########## gets and draws qr code position and angle
        #position, realposition,RotationAngle = qr.conter(current_frame_blank)
        #print(position, realposition, RotationAngle)
        #qr_frame = pl.draw_blank(blank = current_frame_blank, coordinate_list=[position], color = (0, 255, 0))
        
        
        ########## image resize
        # current_frame = cv2.resize(current_frame, tuple([int(1.5 * current_frame.shape[1]), int(1.5 * current_frame.shape[0])]))
        cv2.imshow("green", current_frame_green)
        #cv2.imshow("qrc", mask.sharpen(tq.qrdet(current_frame_b)))
        #cv2.imshow("sharpen", mask.sharpen(current_frame_b))
        ############# image output
        #cv2.imshow("frame", current_frame)
        #cv2.imshow("qr", current_frame_blank)
        #cv2.imshow('cube', blank_img_cube)
        #cv2.imshow('qr',qr_frame)

        if cv2.waitKey(1) == 27:
            break
        
    _capture.release()
