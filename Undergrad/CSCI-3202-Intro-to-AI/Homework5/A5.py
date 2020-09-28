from __future__ import print_function
import os
import matplotlib.pyplot as plt
import keras
import utils
from sklearn.model_selection import train_test_split
from keras.datasets import mnist
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten, Activation
from keras.layers import Conv2D, MaxPooling2D
from keras import backend as K
from keras.applications.vgg16 import VGG16
from keras.preprocessing import image
from keras.applications.vgg19 import preprocess_input
from keras.models import Model
from datetime import datetime
import glob
import random



import numpy as np

from PIL import Image

def getVGGFeatures(fileList, layerName):
	#Initial Model Setup
	base_model = VGG16(weights='imagenet')
	model = Model(inputs=base_model.input, outputs=base_model.get_layer(layerName).output)
	
	#Confirm number of files passed is what was expected
	rArray = []
	print ("Number of Files Passed:")
	print(len(fileList))

	for iPath in fileList:
		#Time Printing for Debug, you can comment this out if you wish
		now = datetime.now()
		current_time = now.strftime("%H:%M:%S")
		print("Current Time =", current_time)
		try:
			#Read Image
			img = image.load_img(iPath)
			#Update user as to which image is being processed
			print("Getting Features " +iPath)
			#Get image ready for VGG16
			img = img.resize((224, 224))
			x = image.img_to_array(img)
			x = np.expand_dims(x, axis=0)
			x = preprocess_input(x)
			#Generate Features
			internalFeatures = model.predict(x)
			rArray.append((iPath, internalFeatures))			
		except:
			print ("Failed "+ iPath)
	return rArray

#def getVGGFeatures(img, layerName):
	#base_model = VGG16(weights='imagenet')
	#odel = Model(inputs=base_model.input, outputs=base_model.get_layer(layerName).output)
	#img = img.resize((224, 224))
	#x = image.img_to_array(img)
	#x = np.expand_dims(x, axis=0)
	#x = preprocess_input(x)

	#internalFeatures = model.predict(x)

	#return internalFeatures

def cropImage(image, x1, y1, x2, y2):
	ima=image.crop((x1,y1,x2,y2))
	return ima

def standardizeImage(image, x, y):
	ima=image.resize((x,y),Image.ANTIALIAS)
	return ima

def preProcessImages(images):
	directory='cropped'
	cc=[]
	if not os.path.exists(directory):
		os.makedirs(directory)
	for i in range(0,len(images)):
		pp=[]
		r=images[i][1]
		w=""
		for j in range(0,len(r)):
			if r[j]==',':
				pp.append(int(w))
				w=""
			elif j==len(r)-1:
				w+=r[j]
				pp.append(int(w))
				w=""
			else:
				w+=r[j]
		c=cropImage(images[i][0],pp[0],pp[1],pp[2],pp[3])
		d=standardizeImage(c,60,60)
		cc.append(d)
		if not os.path.isfile("cropped/"+images[i][2]):
			d=d.save('cropped/'+images[i][2])
	return cc


def visualizeWeight():
	#You can change these parameters if you need to
	utils.raiseNotDefined()

def trainFaceClassifier(preProcessedImages, labels):
	ar=[]
	pr=labels
	for i in range(0,len(preProcessedImages)):
		c=np.array(preProcessedImages[i])
		c=np.std(c)
		c=c.flatten()
		ar.append(c)
		#pr.append(preProcessedImages[i][1])

	train=[]
	valid=[]
	epoc=[]
	epochNum=20
	re=0

	mp=Sequential()
	mp.add(Dense(32,input_shape=(len(ar[0]),),activation="softmax"))
	#mp.add(Flatten())
	mp.add(Dense(6,input_shape=(32,),activation="sigmoid"))
	mp.compile(loss="categorical_crossentropy", optimizer="adam",metrics=["accuracy"])
	
	for i in range(0,epochNum):
		valid1=[]
		valid2=[]
		c=999
		q=0
		X_train,X_test,y_train,y_test=train_test_split(ar,pr,test_size=1/epochNum)
		for j in range(0,epochNum):
			X_tr,X_valid,y_tr, y_valid=train_test_split(np.array(X_train),np.array(y_train),test_size=1/epochNum)
			mp.fit(np.array(X_valid),np.array(y_valid),batch_size=int(len(X_valid)/3), epochs=epochNum)
			e=mp.evaluate(np.array(X_tr),np.array(y_tr))
			print(e)
			if e[0]<c:
				c=e[0]
				q=j
			valid1.append(X_valid)
			valid2.append(y_valid)
		valid.append(c)
		mp.fit(np.array(X_test),np.array(y_test),batch_size=int(len(X_train)/3),epochs=epochNum, validation_data=(valid1[q],valid2[q]))
		e=mp.evaluate(np.array(X_train),np.array(y_train))
		train.append(e[0])

	epoc=np.array(range(0,epochNum))

	fig, ax=plt.subplots(figsize=(8,8))
	ax.plot(epoc,train,'b')
	ax.plot(epoc,valid,'k')
	ax.set_xlabel('Epoch',fontsize=16)
	ax.set_ylabel('Error',fontsize=16)
	plt.show()
	return mp

def trainFaceClassifier2(preProcessedImages, labels):
	ar=[]
	pr=labels
	for i in range(0,len(preProcessedImages)):
		c=np.array(preProcessedImages[i])
		c=np.std(c)
		c=c.flatten()
		ar.append(c)
		#pr.append(preProcessedImages[i][1])

	train=[]
	valid=[]
	epoc=[]
	epochNum=20
	re=0

	mp=Sequential()
	mp.add(Dense(32,input_shape=(len(ar[0]),),activation="softmax"))
	#mp.add(Flatten())
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	mp.add(Dense(32,input_shape=(32,),activation='relu'))
	mp.add(Dropout(0.5))
	#mp.add(Flatten())
	mp.add(Dense(6,input_shape=(32,),activation="sigmoid"))
	mp.compile(loss="categorical_crossentropy", optimizer="adam",metrics=["accuracy"])
	
	for i in range(0,epochNum):
		valid1=[]
		valid2=[]
		c=999
		q=0
		X_train,X_test,y_train,y_test=train_test_split(ar,pr,test_size=1/epochNum)
		for j in range(0,epochNum):
			X_tr,X_valid,y_tr, y_valid=train_test_split(np.array(X_train),np.array(y_train),test_size=1/epochNum)
			mp.fit(np.array(X_valid),np.array(y_valid),batch_size=int(len(X_valid)/3), epochs=epochNum)
			e=mp.evaluate(np.array(X_tr),np.array(y_tr))
			print(e)
			if e[0]<c:
				c=e[0]
				q=j
			valid1.append(X_valid)
			valid2.append(y_valid)
		valid.append(c)
		mp.fit(np.array(X_test),np.array(y_test),batch_size=int(len(X_train)/3),epochs=epochNum, validation_data=(valid1[q],valid2[q]))
		e=mp.evaluate(np.array(X_train),np.array(y_train))
		train.append(e[0])

	epoc=np.array(range(0,epochNum))

	fig, ax=plt.subplots(figsize=(8,8))
	ax.plot(epoc,train,'b')
	ax.plot(epoc,valid,'k')
	ax.set_xlabel('Epoch',fontsize=16)
	ax.set_ylabel('Error',fontsize=16)
	plt.show()
	return mp


def trainFaceClassifier_VGG(extractedFeatures, labels):
	ar=[]
	pr=labels
	for i in range(0,len(extractedFeatures)):
		c=np.array(extractedFeatures[i])
		c=np.std(c)
		c=c.flatten()
		ar.append(c)
		#pr.append(preProcessedImages[i][1])

	train=[]
	valid=[]
	epoc=[]
	epochNum=20
	re=0

	mp=Sequential()
	mp.add(Dense(32,input_shape=(len(ar[0]),),activation="softmax"))
	#mp.add(Flatten())
	mp.add(Dense(6,input_shape=(32,),activation="sigmoid"))
	mp.compile(loss="categorical_crossentropy", optimizer="adam",metrics=["accuracy"])
	
	for i in range(0,epochNum):
		valid1=[]
		valid2=[]
		c=999
		q=0
		X_train,X_test,y_train,y_test=train_test_split(ar,pr,test_size=1/epochNum)
		for j in range(0,epochNum):
			X_tr,X_valid,y_tr, y_valid=train_test_split(np.array(X_train),np.array(y_train),test_size=1/epochNum)
			mp.fit(np.array(X_valid),np.array(y_valid),batch_size=int(len(X_valid)/3), epochs=epochNum)
			e=mp.evaluate(np.array(X_tr),np.array(y_tr))
			print(e)
			if e[0]<c:
				c=e[0]
				q=j
			valid1.append(X_valid)
			valid2.append(y_valid)
		valid.append(c)
		mp.fit(np.array(X_test),np.array(y_test),batch_size=int(len(X_train)/3),epochs=epochNum, validation_data=(valid1[q],valid2[q]))
		e=mp.evaluate(np.array(X_train),np.array(y_train))
		train.append(e[0])

	epoc=np.array(range(0,epochNum))

	fig, ax=plt.subplots(figsize=(8,8))
	ax.plot(epoc,train,'b')
	ax.plot(epoc,valid,'k')
	ax.set_xlabel('Epoc',fontsize=16)
	ax.set_ylabel('Error',fontsize=16)
	plt.show()
	return mp


if __name__ == '__main__':
	#Using ideas provided in getData to make this a bit more intuitive
	acts= ['Gerard Butler', 'Daniel Radcliffe', 'Michael Vartan', 'Lorraine Bracco', 'Peri Gilpin', 'Angie Harmon']
	act=[0,1,2,3,4,5]
	im=[]
	lab=[]
	fil=sorted(glob.glob('cropped/*'))
	e='downloadedFiles.txt'

	ee=0
	for a in acts:
		i=0
		c=0
		name = a.split()[1].lower()
		for line in open(e):
			if line.split()[1]==a.split()[1]:
				file=name+str(i)+'.jpg'
				if os.path.isfile("uncropped/"+file):
					i+=1
					im.append((Image.open('uncropped/'+file).convert('L'),line.split()[5],file))
					lab.append(act[ee])
				elif os.path.isfile('uncropped/'+name+str(i+1)+'.jpg'):
					i+=1
				else:
					c=1
		ee+=1

	print(len(im))
	imm=preProcessImages(im)
	labb=keras.utils.to_categorical(lab)
	immm=trainFaceClassifier2(imm,labb)
	#immm=trainFaceClassifier(imm,labb)
	#while len(lab)<len(fil):
		#lab.append(random.randint(0,5))
	#labb=keras.utils.to_categorical(lab)
	#p=getVGGFeatures(fil,'block3_pool')
	#ccc=[]
	#for i in range(0,len(p)):
		#ccc.append(p[i][1])
	#trainFaceClassifier_VGG(ccc,labb)