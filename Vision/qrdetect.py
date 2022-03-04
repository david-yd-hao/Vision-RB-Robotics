import cv2
import math
import calibrate
import poly as pl


def detecte(image):
    '''提取所有轮廓'''
    gray=cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)#// Convert Image captured from Image Input to GrayScale	
    canny = cv2.Canny(gray, 100, 200,3)#Apply Canny edge detection on the gray image
    contours,hierachy=cv2.findContours(canny,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)#Find contours with hierarchy
    return contours,hierachy
    #coutour是一个list，每个元素都是一个轮廓（彻底围起来算一个轮廓），用numpy中的ndarray表示。
    #hierarchy也是一个ndarry，其中的元素个数和轮廓个数相同，每个轮廓contours[i]对应4个hierarchy元素hierarchy[i][0] ~hierarchy[i][3]，
    #分别表示后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号，如果没有对应项，则该值为负数。


def cv_distance(a,b):
    #求出两点的截距
   selfx=a[0]-b[0]
   selfy=a[1]-b[1]
   selflen= math.sqrt((selfx**2)+(selfy**2))
   return selflen


def conter(image):
    # 输入包含二维码的图片，返回二维码的实际位置以及朝向

    contours,hierarchy=detecte(image)
    # print(len(contours))
    centers = {}
    M = {}
    # lista = [0,1,2,float("nan"),4]
    # print(lista[3])

    """
    Calculate the centers of the contours
    :param contours: Contours detected with find_contours
    :return: object centers as numpy array
    """
    for i in range(len(contours)):
        M[i] = cv2.moments(contours[i])
        if(M[i]["m00"] == 0):
            centers[i] = (float("nan"), float("nan"))
        else:
            centers[i] = (float(M[i]["m10"] / M[i]["m00"]), float(M[i]["m01"] / M[i]["m00"]))
        # print(centers[i])
  #计算符合5层嵌套的回形框
    mark = 0
    hierarchy = hierarchy[0]
    # print(hierarchy[0])

    for i in range(len(contours)):
        k = i
        c = 0
        while hierarchy[k][2] != -1:
            k = hierarchy[k][2]
            c = c+1
        if hierarchy[k][2] != -1:
            c = c+1
        if c >= 5:
            if mark == 0 :A = i
            elif mark == 1 :B = i
            elif mark == 2 :C = i
            mark = mark + 1
    # print(mark)
    #给返回值赋初值，如果没有识别到三个回形框，返回-1
    CentralPoint = [-1,-1]
    realPosition = [-1,-1]
    RotationAngle = -1
    if mark >=3:
        #计算三个回形框的质心点之间的距离
        AB = cv_distance(centers[A],centers[B])
        BC = cv_distance(centers[B],centers[C])
        CA = cv_distance(centers[C],centers[A])
        # print(AB)
        # print(BC)
        # print(CA)
        # print("three control points:",centers[A],centers[B],centers[C])
        #最长的斜边是right点和bottom点的连线，另一个点即为top点
        if AB > BC and AB > CA:
            outlier = C
            median1 = A
            median2 = B
        elif CA > AB and CA > BC:
            outlier = B
            median1 = A
            median2 = C
        elif BC > AB and BC > CA:
            outlier = A
            median1 = B
            median2 = C
        top = outlier
        # print("top point:",centers[top])
        
        #斜边的的中点就是二维码的质心
        CentralPoint_x = int((centers[median1][0]+centers[median2][0])/2)
        CentralPoint_y = int((centers[median1][1]+centers[median2][1])/2)
        CentralPoint = [CentralPoint_y,CentralPoint_x]
        # print("Central point:",CentralPoint)
        #根据图片像素与实际map的比例，可以求出二维码质心在实际中的位置
        #图片像素1920*1080  实际距离是4.2*2.4m
        realPosition_x =  (CentralPoint_x)
        realPosition_y =  (CentralPoint_y)
        realPosition = [realPosition_x,realPosition_y]
        # print("real point:",realPosition)



        # //判断二维码旋转方向，通过求top点在对角线哪一侧
        # // 定义：平面上的三点A(x1,y1),B(x2,y2),C(x3,y3)的面积量：
        # // S(A,B,C)=|A B C|= (x1-x3)*(y2-y3)-(y1-y3)*(x2-x3)
        # // 令矢量的起点为A，终点为B，判断的点为C， 
        # // 如果S（A，B，C）为正数，则C在矢量AB的左侧； 
        # // 如果S（A，B，C）为负数，则C在矢量AB的右侧； 
        # // 如果S（A，B，C）为0，则C在直线AB上
        # 	//定义一个位于二维码左上方各200个像素的的DefaultTopPoint，#这样算出来的0度方向是水平向右
        # //当二维码旋转0度时，top点一定在此DefaultTopPoint与二维码质心CentralPoint的连线上
      
        DefaultTopPoint_x = CentralPoint_x + 200
        DefaultTopPoint_y = CentralPoint_y - 200
        DefaultTopPoint = [DefaultTopPoint_x,DefaultTopPoint_y]
        #沿逆时针方向增大到359
        Sdirection = (DefaultTopPoint_x - centers[top][0]) * (CentralPoint_y - centers[top][1]) -  (DefaultTopPoint_y - centers[top][1]) * (CentralPoint_x - centers[top][0])
        
        if Sdirection == 0:
            if centers[top][0]<CentralPoint_x: RotationAngle = 0#关键点的x坐标小于旋转中心的x坐标
            else:RotationAngle = 180
        else: 
            # //通过余弦定理，已知三边求角度
            aa = cv_distance(DefaultTopPoint,centers[top])
            bb = cv_distance(centers[top],CentralPoint)
            cc = cv_distance(CentralPoint,DefaultTopPoint)
            RotationAngle =  math.degrees(math.acos((aa*aa-bb*bb-cc*cc)/(-2*bb*cc)))#旋转角
            if Sdirection < 0: RotationAngle = 360-RotationAngle
        # print("RotationAngle:",RotationAngle)
    return CentralPoint, realPosition,RotationAngle


def run():
    print("start Reveive")
    cap = cv2.VideoCapture('http://localhost:8081/stream/video.mjpeg')
    while True:
        ret, frame = cap.read()
        frame = calibrate.undistort_fisheye(frame)
        # frame=cv2.imread("/Users/davidhao/GitProject/CamProject/Vision-RB-Robotics/TestPics/qr.png")
        position, realposition,RotationAngle = conter(frame)
        if position == [-1,-1]:
            continue
        print(position, realposition, RotationAngle)
        frame = pl.draw_blank(blank = frame, coordinate_list=[position], color = (0, 255, 0))
        cv2.imshow('123456789',frame)
        if cv2.waitKey(1) == 27:
            break


if __name__ == '__main__':

    run()
    pass