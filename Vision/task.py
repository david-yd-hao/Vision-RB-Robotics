import cv2
import threading
import numpy as np
import calibrate
import contours
import poly as pl
import mask
import cubes
import qrdetect as qr

cap = cv2.VideoCapture("http://localhost:8081/stream/video.mjpeg")



if __name__ == "__main__":
    while True:
        ret, frame = cap.read()
        frame = calibrate.undistort_fisheye(frame)
        frame_copy = frame.copy()
        frame_left_red = mask.red_mask(frame[500:759, 0:400])   ###### y axis from top, x axis from left
        blank = np.zeros(shape=frame.shape, dtype=np.uint8)
