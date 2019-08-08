import cv2
import numpy as np
import re

trainingset = (1, 3, 4, 5, 7, 9)
testingset = (2, 6, 8, 10)
kset = (1, 2, 3, 6, 10, 20, 30)
images = [0] * 6
data_matrix = []
for i in range(6):
	images[i] = [0] * 10

#import images
for i in range(10):
	for s in trainingset:
		img = cv2.imread("/DCNFS/users/student/ppaulson/coen166/att_faces_10/s" + str(i+1) + '/' + str(s) + '.pgm', 0)
		if img is None:
			print("Error: import image s" + str(i+1) + "#" + str(s) + " failed.")
			exit(0)
		flat = img.flatten()
		data_matrix.append(flat)

#PCA
for k in kset:
	mean, eigenvectors = cv2.PCACompute(np.array(data_matrix), mean=None, maxComponents = k)
	#Project onto rank-k subspace
	output = cv2.PCAProject(np.array(data_matrix), mean, eigenvectors)
	#Train KNN
	knn = cv2.KNearest()
	responses = []
	for i in range(10):
		for j in range(6):
			responses.append([i+1])
	knn.train(output, np.array(responses))
	#Evaluate testing images
	print("K:" + str(k) + "\n")
	numCorrect = 0;
	for i in range(10):
		for s in testingset:
			img = cv2.imread("/DCNFS/users/student/ppaulson/coen166/att_faces_10/s" + str(i+1) + '/' + str(s) + '.pgm', 0)
			if img is None:
				print("Error: import image s" + str(i+1) + "#" + str(s) + " failed.")
				exit(0)
			img = img.flatten()
			img = np.array(img)
			img = cv2.PCAProject(img.reshape((1, 10304)), mean, eigenvectors)
			ret, label, nearest_neighbors, distance = knn.find_nearest(img, 3)
			label_as_int = re.search('\d+', str(label)).group(0)
			if int(label_as_int) == i+1:
				numCorrect = numCorrect + 1
			print("Label:" + str(label) + " Expected:" + str(i+1))
	print("Accuracy for K=" + str(k) + " was " + str(100 * (numCorrect / 40.)) + "%")
