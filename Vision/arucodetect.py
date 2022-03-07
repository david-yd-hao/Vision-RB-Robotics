import numpy as np
import time
import cv2
import cv2.aruco as aruco
import threading
import numpy as np
import calibrate
import contours
import poly as pl
import mask
import cubes



#num = 0
def detectaruco(frame, mtx, dist,font):

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    aruco_dict = aruco.Dictionary_get(aruco.DICT_6X6_250)
    parameters =  aruco.DetectorParameters_create()

    '''
    detectMarkers(...)
        detectMarkers(image, dictionary[, corners[, ids[, parameters[, rejectedI
        mgPoints]]]]) -> corners, ids, rejectedImgPoints
    '''

    #使用aruco.detectMarkers()函数可以检测到marker，返回ID和标志板的4个角点坐标
    corners, ids, rejectedImgPoints = aruco.detectMarkers(gray,aruco_dict,parameters=parameters)
    print(corners)
    frame = pl.draw_points_xy(frame,[corners[0][0][0]],(255,0,0))
    frame = pl.draw_points_xy(frame,[corners[0][0][1]],(0,0,255))

#    如果找不打id
    if ids is not None:

        
        ###### DRAW ID #####
        cv2.putText(frame, "Id: " + str(ids), (0,64), font, 1, (0,255,0),2,cv2.LINE_AA)


    else:
        ##### DRAW "NO IDS" #####
        cv2.putText(frame, "No Ids", (0,64), font, 1, (0,255,0),2,cv2.LINE_AA)

    return frame


if __name__ == "__main__":

    mtx = np.array([[637.8931714029114, 0.0, 509.67125143385334], [0.0, 636.4000140079311, 371.2613659540199], [0.0, 0.0, 1.0]])
    dist = np.array([[-0.02628723220492124], [-0.1740869162806197], [0.11587794888959864], [0.041124156040405195]])
    font = cv2.FONT_HERSHEY_SIMPLEX #font for displaying text (below)

    frame = cv2.imread(r'E:\Git\CambridgeProjects\Vision-RB-Robotics\Vision\TestPics\aruco\UDT_imaged1.png')
    frame = detectaruco(frame,mtx,dist,font)
    cv2.imshow("frame",frame)

    cv2.waitKey(0)
