import cv2
import numpy as np
import os
import glob


def calibrate():
    CHECKERBOARD = (6,9)
    subpix_criteria = (cv2.TERM_CRITERIA_EPS+cv2.TERM_CRITERIA_MAX_ITER, 30, 0.1)
    calibration_flags = cv2.fisheye.CALIB_RECOMPUTE_EXTRINSIC+cv2.fisheye.CALIB_CHECK_COND+cv2.fisheye.CALIB_FIX_SKEW
    objp = np.zeros((1, CHECKERBOARD[0]*CHECKERBOARD[1], 3), np.float32)
    objp[0,:,:2] = np.mgrid[0:CHECKERBOARD[0], 0:CHECKERBOARD[1]].T.reshape(-1, 2)
    _img_shape = None
    objpoints = [] # 3d point in real world space
    imgpoints = [] # 2d points in image plane.
    images = glob.glob('*.png')
    print(images)
    for fname in images:
        img = cv2.imread(fname)
        if _img_shape == None:
            _img_shape = img.shape[:2]
        else:
            assert _img_shape == img.shape[:2], "All images must share the same size."
        gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
        # Find the chess board corners
        ret, corners = cv2.findChessboardCorners(gray, CHECKERBOARD, cv2.CALIB_CB_ADAPTIVE_THRESH+cv2.CALIB_CB_FAST_CHECK+cv2.CALIB_CB_NORMALIZE_IMAGE)
        # If found, add object points, image points (after refining them)
        if ret == True:
            objpoints.append(objp)
            cv2.cornerSubPix(gray,corners,(3,3),(-1,-1),subpix_criteria)
            imgpoints.append(corners)
    N_OK = len(objpoints)
    K = np.zeros((3, 3))
    D = np.zeros((4, 1))
    rvecs = [np.zeros((1, 1, 3), dtype=np.float64) for i in range(N_OK)]
    tvecs = [np.zeros((1, 1, 3), dtype=np.float64) for i in range(N_OK)]
    rms, _, _, _, _ = \
        cv2.fisheye.calibrate(
            objpoints,
            imgpoints,
            gray.shape[::-1],
            K,
            D,
            rvecs,
            tvecs,
            calibration_flags,
            (cv2.TERM_CRITERIA_EPS+cv2.TERM_CRITERIA_MAX_ITER, 30, 1e-6)
        )
    print("Found " + str(N_OK) + " valid images for calibration")
    print("DIM=" + str(_img_shape[::-1]))
    print("K=np.array(" + str(K.tolist()) + ")")
    print("D=np.array(" + str(D.tolist()) + ")")
    DIM=_img_shape[::-1]
    K=np.array(K)
    D=np.array(D)
    return K, D, DIM

def undistort_fisheye(img, K=np.array([[637.8931714029114, 0.0, 509.67125143385334], [0.0, 636.4000140079311, 371.2613659540199], [0.0, 0.0, 1.0]]), D=np.array([[-0.02628723220492124], [-0.1740869162806197], [0.11587794888959864], [0.041124156040405195]]), DIM=(1016, 760)):
    img = rotate_image(img, -3)
    h,w = img.shape[:2]
    cam = np.eye(3)
    DIM = (int(w * 1.1), int(h * 1.1))
    map1, map2 = cv2.fisheye.initUndistortRectifyMap(K, D, cam, K, DIM, cv2.CV_16SC2)
    undistorted_img = cv2.remap(img, map1, map2, interpolation=cv2.INTER_LINEAR, borderMode=cv2.BORDER_DEFAULT)
    undistorted_img = undistorted_img[0:816, 90:987]
    return undistorted_img
def undi(img, K=np.array([[637.8931714029114, 0.0, 509.67125143385334], [0.0, 636.4000140079311, 371.2613659540199], [0.0, 0.0, 1.0]]), D=np.array([[-0.02628723220492124], [-0.1740869162806197], [0.11587794888959864], [0.041124156040405195]]), DIM=(1016, 760)):
    h,w = img.shape[:2]
    cam = np.eye(3)
    newcammatrix, _ = cv2.getOptimalNewCameraMatrix(K, D, DIM[::-1], 1, DIM[::-1])
    return cv2.undistort(img, K, D, None, newcammatrix)

def undistort(img, K=np.array([[637.8931714029114, 0.0, 509.67125143385334], [0.0, 636.4000140079311, 371.2613659540199], [0.0, 0.0, 1.0]]), D=np.array([[-0.02628723220492124], [-0.1740869162806197], [0.11587794888959864], [0.041124156040405195]]), DIM=(1016, 760)):
    return cv2.undistort(img, K, D, None, K)
# calibrate camera, output parameter, and undistort val.jpg

def rotate_image(image, angle):
  image_center = tuple(np.array(image.shape[1::-1]) / 2)
  rot_mat = cv2.getRotationMatrix2D(image_center, angle, 1.0)
  result = cv2.warpAffine(image, rot_mat, image.shape[1::-1], flags=cv2.INTER_LINEAR)
  return result
if __name__ == "__main__":
    img = cv2.imread("./TestPics/cube/DT_imaged1.png")
    cv2.imshow("pic", undistort_fisheye(img))
    img = undistort_fisheye(img)
    cv2.imwrite("./TestPics/cube/UDT_imaged1.png", img)
    cv2.waitKey(0)