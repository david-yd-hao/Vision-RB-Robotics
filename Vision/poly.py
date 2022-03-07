from ast import Not
import cv2
import contours


def get_average(polygons):
    average_points = []
    for i in polygons:
        sum_x = 0
        sum_y = 0
        count = 0
        for j in i:
            sum_x += j[0][0]
            sum_y += j[0][1]
            count += 1
        average_points.append((int(sum_x/count), int(sum_y/count)))
    return average_points


def get_poly(pic):
    img = pic

    font = cv2.FONT_HERSHEY_COMPLEX

    contour = contours.get_contour(img)
    polygons_list = []
    for cnt in contour : 

        approx = cv2.approxPolyDP(cnt, 0.001 * cv2.arcLength(cnt, True), True)
        polygons_list.append(approx)

    return polygons_list


############ draw coordinate list on img picture
############ (y,x)
def draw_points_yx(img, coordinate_list, color = (0, 255, 0)):
    if coordinate_list is not None:
        img = img.copy()
        for i in coordinate_list:
            y = int(i[0])
            x = int(i[1])
            img = cv2.circle(img, (x,y), radius=0, color=color, thickness=3) 
        return img
    else:
        return img


############ (x,y)
def draw_points_xy(img, coordinate_list, color = (0, 255, 0)):
    if coordinate_list is not None:
        img = img.copy()
        for i in coordinate_list:
            x = int(i[0])
            y = int(i[1])
            img = cv2.circle(img, (x,y), radius=0, color=color, thickness=3) 
        return img
    else:
        return img


#def draw_line_grad(img, coordinate, grad = , color = (0, 255, 0))