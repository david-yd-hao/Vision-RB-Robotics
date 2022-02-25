from calendar import c
import cv2
import threading
import numpy as np
import calibrate
import contours
import poly as pl
import mask
import coordinates
import cubes


########## select camera
_capture = cv2.VideoCapture("http://localhost:8081/stream/video.mjpeg")
_capture.set(cv2.CAP_PROP_BUFFERSIZE, 0)

########### video writer
# out = cv2.VideoWriter('output_1030.avi', cv2.VideoWriter_fourcc(*"MJPG"), 20.0, (1016,760))
# _npzfile = np.load('cache.npz')
#K2 = _npzfile['mtx']
#D2 = _npzfile['dist']

######### set number for image capturer
imagenumber = 0

if _capture.isOpened(): 
    _rval, _frame  = _capture.read()
else:
    _rval = False
_grey = None


def _reader_func():
    global _frame, _grey
    while True:
        import time
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


def get_grey(copy_=True):
    global _grey
    if copy_ and _grey is not None: return _grey.copy()
    else: return _grey


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
        current_frame_left_red = mask.red_mask(current_frame[500:759, 0:400])   ###### y axis from top, x axis from left
        current_frame_right_red = mask.red_mask(current_frame[100:450,600:950])
        cv2.imshow('left',current_frame_left_red)

        blank_image = np.zeros(shape=current_frame_left_red.shape, dtype=np.uint8)
        blank_copy = np.zeros(shape=current_frame.shape, dtype=np.uint8)
        ######### gets and draws boundary of contours on original
        current_contour = contours.get_contour(current_frame)
        current_left_contours = contours.get_contour(current_frame_left_red)
        current_right_contours = contours.get_contour(current_frame_right_red)
        ######### gets and draws cubes on blank
        cube = cubes.getcube(blank_image, current_left_contours, 0, 500)
        dest = cubes.getcube(blank_copy, current_right_contours, 600, 100)
        print(cube)
        print(dest)
        cv2.drawContours(current_frame, current_contour, -1, (0, 255, 0), thickness=1)

        ########### get and draws corner dots and center dots
        # plist, current_frame = corners.draw_corners(current_frame)
        polygons = pl.get_poly(current_frame_right_red)
        average = pl.get_average(polygons)
        avg_points_over_time.append(average)

        #### get red destinations' coordinates
        i += 1
        if i == calibrate_frame_num:
            const_pts, exist = coordinates.get_const_points(avg_points_over_time, 3)
            if exist:
                dist1 = coordinates.get_dist(const_pts[0], const_pts[1])
                dist2 = coordinates.get_dist(const_pts[1], const_pts[2])
                dist3 = coordinates.get_dist(const_pts[2], const_pts[0])
                a = sorted([dist1, dist2, dist3])
                if a[0] == dist1:
                    const_pts = [const_pts[0], const_pts[2]]
                if a[0] == dist2:
                    const_pts = [const_pts[1], const_pts[0]]
                if a[0] == dist3:
                    const_pts = [const_pts[1], const_pts[2]]
            red_destinations = const_pts
            red_destinations[0] = tuple(map(sum, zip(red_destinations[0], (600, 100))))
            red_destinations[1] = tuple(map(sum, zip(red_destinations[1], (600, 100))))
        if i > calibrate_frame_num:
            pl.draw_blank(current_frame, red_destinations, (0, 0, 255))
        # print(red_destinations)

        # current_frame = corners.draw_points(current_frame, average, 2, (0, 255, 0))
        # blank_image = corners.draw_points(blank_image, average, 2, (0, 255, 0))
        
        
        
        ########## image resize
        current_frame = cv2.resize(current_frame, tuple([int(1.5 * current_frame.shape[1]), int(1.5 * current_frame.shape[0])]))
        
        
        ############# image output
        cv2.imshow("red", current_frame)

        #out.write(get_frame())
    _capture.release()
    # out.release()
