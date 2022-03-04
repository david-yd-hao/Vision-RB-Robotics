import cv2
import imutils

def qrdet(img):
    decoder = cv2.QRCodeDetector()
    rv, points = decoder.detect(img)
    if rv:
    
        points = points[0]
        for i in range(len(points)):
            pt1 = [int(val) for val in points[i]]
            pt2 = [int(val) for val in points[(i + 1) % 4]]
            cv2.line(img, pt1, pt2, color=(255, 0, 0), thickness=3)
    
    return img