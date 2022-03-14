import numpy as np
import cv2
import cv2.aruco as aruco
import numpy as np
import poly as pl



#num = 0
def detectaruco(frame,font):

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
    edge_center = [0,0]
    centerline_rotation = 0
    edge_rotation = 0
    if corners:
        frame = pl.draw_points_xy(frame,[corners[0][0][0]],(255,0,0))
        frame = pl.draw_points_xy(frame,[corners[0][0][1]],(0,0,255))
        first_point = corners[0][0][0]
        second_point = corners[0][0][1]
        dx = second_point[0] - first_point[0]
        dy = first_point[1] - second_point[1]
        d = np.sqrt(dx**2+dy**2)
        if dx > 0:
            edge_rotation = np.arcsin(dy/d)*180/np.pi
        elif dx <0:
            edge_rotation = 180 - (np.arcsin(dy/d)*180/np.pi)
        centerline_rotation = edge_rotation + 90

        edge_center = [int((corners[0][0][0][0]+corners[0][0][1][0])/2), int((corners[0][0][0][1]+corners[0][0][1][1])/2)]


#    如果找不打id
    if ids is not None:

        
        ###### DRAW ID #####
        cv2.putText(frame, "Id: " + str(ids), (0,64), font, 1, (0,255,0),2,cv2.LINE_AA)


    else:
        ##### DRAW "NO IDS" #####
        cv2.putText(frame, "No Ids", (0,64), font, 1, (0,255,0),2,cv2.LINE_AA)

    return frame, corners, edge_center, centerline_rotation


if __name__ == "__main__":

    font = cv2.FONT_HERSHEY_SIMPLEX #font for displaying text (below)

    frame = cv2.imread('/Users/davidhao/GitProject/CamProject/Vision-RB-Robotics/Vision/UDT_imaged1.png')
    frame, corners, edge_center, centerline_rotation = detectaruco(frame,font)
    print(edge_center)
    print(centerline_rotation)
    frame = pl.draw_line_rot(frame,edge_center,centerline_rotation)
    
    cv2.imshow("frame",frame)
    cv2.waitKey(0)
