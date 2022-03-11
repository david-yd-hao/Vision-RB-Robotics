from re import X
import cv2
import matplotlib.pyplot as plt
import calibrate
from sklearn.cluster import *
import numpy as np
from cubes import *
from contours import *


def centroid_histogram(clt):
	# grab the number of different clusters and create a histogram
	# based on the number of pixels assigned to each cluster
	numLabels = np.arange(0, len(np.unique(clt.labels_)) + 1)
	(hist, _) = np.histogram(clt.labels_, bins = numLabels)
	# normalize the histogram, such that it sums to one
	hist = hist.astype("float")
	hist /= hist.sum()
	# return the histogram
	return hist
def plot_colors(hist, centroids):
	# initialize the bar chart representing the relative frequency
	# of each of the colors
	bar = np.zeros((50, 300, 3), dtype = "uint8")
	startX = 0
	# loop over the percentage of each cluster and the color of
	# each cluster
	for (percent, color) in zip(hist, centroids):
		# plot the relative percentage of each cluster
		endX = startX + (percent * 300)
		cv2.rectangle(bar, (int(startX), 0), (int(endX), 50),
			color.astype("uint8").tolist(), -1)
		startX = endX
	
	# return the bar chart
	return bar
def getboundbox(blank_img, contours):
	blank_img_copy = blank_img.copy()
	xt = 0
	yt = 0
	wt = 0
	ht = 0
	n = 0
	rect = []
	for i in range(0,len(contours)):
		x, y, w, h = cv2.boundingRect(contours[i])
        ##### skip rect too large or too small
		area = w * h
		if(area > 150 and area < 500):
			cv2.rectangle(blank_img_copy,(x,y),(x+w,y+h),(0,255,0),2)
			xt += x
			yt += y
			wt += w
			ht += h
			n += 1
	rect.append(int(xt/n))
	rect.append(int(yt/n))
	rect.append(int(wt/n))
	rect.append(int(ht/n))
	rect.append(n)
	return blank_img_copy, rect
img = cv2.imread("./TestPics/cube/DT_imaged1.png")
img = calibrate.undistort_fisheye(img)
img = img[600:800, 20:210]
# img=cv2.cvtColor(img,cv2.COLOR_BGR2RGB)
# img = img.reshape((img.shape[0] * img.shape[1], 3))
###############
# clt = KMeans(n_clusters = 15)
# clt.fit(img)
# hist = centroid_histogram(clt)
# bar = plot_colors(hist, clt.cluster_centers_)
# # show our color bart
# plt.figure()
# plt.axis("off")
# plt.imshow(bar)
# plt.show()
###############
# md=[]
# for i in range(1,10):
#   kmeans=KMeans(n_clusters=i)
#   kmeans.fit(img)
#   o=kmeans.inertia_
#   md.append(o)
# print(md)
# plt.plot(list(np.arange(1,10)),md)
# plt.show()
############
# img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
# cv2.imshow("pic", img)
# cv2.waitKey(0)
# Z = img.reshape((-1,3))
# Z = np.float32(Z)
# # define criteria, number of clusters(K) and apply kmeans()
# criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
# K = 8
# ret,label,center=cv2.kmeans(Z,K,None,criteria,10,cv2.KMEANS_RANDOM_CENTERS)
# A = Z[label.ravel()==0]
# B = Z[label.ravel()==1]
# # Plot the data
# plt.scatter(A[:,0],A[:,1])
# plt.scatter(B[:,0],B[:,1],c = 'r')
# # plt.scatter(center[:,0],center[:,1],s = 80,c = 'y', marker = 's')
# plt.xlabel('Height'),plt.ylabel('Weight')
# plt.show()
###################
cts = get_contour(img)
blank_img = np.zeros(shape=img.shape, dtype=np.uint8)
blank_img_copy, rect= getboundbox(blank_img, cts)
[x, y, w, h, i] = rect
img = img[y:y+h, x:x+w]
cv2.imshow("pic", img)
img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
img = img.reshape((img.shape[0] * img.shape[1], 3))
clt = KMeans(n_clusters = 4)
clt.fit(img)
hist = centroid_histogram(clt)
bar = plot_colors(hist, clt.cluster_centers_)
# show our color bart
plt.figure()
plt.axis("off")
plt.imshow(bar)
plt.show()
