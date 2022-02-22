import cv2
import numpy as np
import contours
import imutils
import poly as pl

font = cv2.FONT_HERSHEY_COMPLEX 

# Read the original image
# img = cv2.imread('/Users/davidhao/GitProject/CamProject/Vision-RB-Robotics/Pics/tablecube_UDT1.png')
#img = img[600:750, 50:250]
# img[:, :, 0:2] = 0


def draw_corners(img):
    polygons_list = pl.get_poly(img)
    plist = []
    for polygon in polygons_list:
        polygons = polygon.ravel()
        i=0
        for j in polygons : 
                if(i % 2 == 0): 
                    x = polygons[i] 
                    y = polygons[i + 1] 
                    plist.append([x, y])
                    # String containing the co-ordinates. 
                    string = str(x) + " " + str(y) 

                    # put dots on corners
                    if(i == 0): 
                        # dot on topmost co-ordinate. 
                        img = cv2.circle(img, (x,y), radius=0, color=(255, 0, 0), thickness=-1) 
                    else: 
                        # dot on remaining co-ordinates. 
                        img = cv2.circle(img, (x,y), radius=0, color=(0, 0, 255), thickness=-1) 
                i = i + 1
    return plist, img
def draw_points(img, points, radius, color):
    for i in points:
        img = cv2.circle(img, (i[0],i[1]), radius=radius, color=color, thickness=radius) 
    return img

if __name__ == "__main__":
    # Showing the final image. 
    cv2.imshow('image2', img)


    # cv2.imshow('pic',contour)
    cv2.waitKey(0)


    # print(contour)