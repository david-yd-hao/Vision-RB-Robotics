from re import I, X
from tkinter import N
import cv2
import matplotlib.pyplot as plt
import calibrate
from sklearn.cluster import *
import numpy as np
from cubes import *
from contours import *
import poly
import mask as mk
import glob
from sklearn.svm import SVC
import matplotlib
import random
random.seed(42)


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

		if(area > 300 and area < 1100):
			cv2.rectangle(blank_img_copy,(x,y),(x+w,y+h),(0,255,0),2)
			xt += x
			yt += y
			wt += w
			ht += h
			n += 1
			print(area)
	if n != 0:
		rect.append(int(xt/n))
		rect.append(int(yt/n))
		rect.append(int(wt/n))
		rect.append(int(ht/n))
		rect.append(n)
	else:
		rect.append(None)
		rect.append(None)
		rect.append(None)
		rect.append(None)
		rect.append(None)
	return blank_img_copy, rect


def trainSV():
	features = []
	labels = []
	for name in glob.glob("./TestPics/cube/*.png"):
		print(name)
		colour = 0
		img = cv2.imread(name)
		name = name.split('/')[-1]
		if name[:1] == "B":
			colour = 1
		img = img[600:800, 40:210]
		img = mk.white_mask(img)
		cts = get_contour(img)
		blank_img = np.zeros(shape=img.shape, dtype=np.uint8)
		blank_img_copy, rect= getboundbox(blank_img, cts)
		[x, y, w, h, i] = rect
		img = img[y:y+h, x:x+w]
		# cv2.imshow("pic", cv2.resize(img, (img.shape[1] * 10, img.shape[0] * 10)))
		# cv2.waitKey(0)
		img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
		img = img.reshape((img.shape[0] * img.shape[1], 3))
		clt = KMeans(n_clusters = 3)
		clt.fit(img)
		features.append(clt.cluster_centers_)
		labels.append(colour)
	sv = SVC(kernel="poly", random_state=42)
	nsamples, nx, ny = np.array(features).shape
	features = np.array(features).reshape((nsamples,nx*ny))
	print(features, labels)
	sv.fit(features,labels)
	return sv

def predictSV(sv, img):
	img = img[600:800, 40:210]
	img = mk.white_mask(img)
	cts = get_contour(img)
	blank_img = np.zeros(shape=img.shape, dtype=np.uint8)
	blank_img_copy, rect= getboundbox(blank_img, cts)
	[x, y, w, h, i] = rect
	img = img[y:y+h, x:x+w]
	# cv2.imshow("pic", cv2.resize(img, (img.shape[1] * 10, img.shape[0] * 10)))
	# cv2.waitKey(0)
	img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
	img = img.reshape((img.shape[0] * img.shape[1], 3))
	clt = KMeans(n_clusters = 3)
	clt.fit(img)
	test = []
	for i in clt.cluster_centers_:
		for j in range(len(i)):
			test.append(i[j])
	test = [test]
	return sv.predict(test)
# print(test)
# result = sv.predict(test)
# print(result)


if __name__ == "__main__":
	sv = trainSV()
	img = cv2.imread('./TestPics/cube/val/RUDT_imaged11.png')
	result = predictSV(sv,img)
	print(result)





# show our color bart
# md=[]
# for i in range(1,10):
#   kmeans=KMeans(n_clusters=i)
#   kmeans.fit(img)
#   o=kmeans.inertia_
#   md.append(o)
# kn = kl.KneeLocator(list(np.arange(1,10)), md, curve='convex', direction='decreasing')
# print(kn.knee)
