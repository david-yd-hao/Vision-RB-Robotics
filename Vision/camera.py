import cv2
import threading
import test
import numpy as np

# select camera
_capture = cv2.VideoCapture("http://localhost:8081/stream/video.mjpeg")
_capture.set(cv2.CAP_PROP_BUFFERSIZE, 0)

# undistortion parameters
K2 = np.array([[637.8931714029114, 0.0, 509.67125143385334], [0.0, 636.4000140079311, 371.2613659540199], [0.0, 0.0, 1.0]])
D2 = np.array([[-0.02628723220492124], [-0.1740869162806197], [0.11587794888959864], [0.041124156040405195]])
DIM2 = (1016, 760)

#out = cv2.VideoWriter('output_1030.avi', cv2.VideoWriter_fourcc(*"MJPG"), 20.0, (1016,760))
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
    while(True):
        if cv2.waitKey(1) == 27:
            break
        if cv2.waitKey(1) == 32:
            imagenumber += 1
            name = str(imagenumber)
            cv2.imwrite('imaged%s.png'%name, test.undistort(_frame, K2, D2, DIM2))

        cv2.imshow("frame", test.undistort(get_frame(), K2, D2, DIM2))
        #out.write(get_frame())
    _capture.release()
    out.release()