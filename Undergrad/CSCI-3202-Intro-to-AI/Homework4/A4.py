import numpy as np
import pandas as pd
import random
import matplotlib.pyplot as plt
from sklearn.neural_network import MLPClassifier
import utils


#Perfect Instances
five =  [0,1,1,1,0, 0,1,0,0,0, 0,1,1,1,0, 0,0,0,1,0, 0,1,1,1,0]
two = [0,1,1,1,0, 0,0,0,1,0, 0,1,1,1,0, 0,1,0,0,0, 0,1,1,1,0]
patterns = [five,two]

def loadGeneratedData():
	print("TODO")

def distort_input(instance, percent_distortion):

	#percent distortion should be a float from 0-1
	#Should return a distorted version of the instance, relative to distoriton Rate
	for i in range(0,len(instance)):
		e=random.uniform(0,1)
		if e<percent_distortion:
			if instance[i]==0:
				instance[i]=1
			else:
				instance[i]=0
	return instance





class HopfieldNetwork:
	def __init__(self, size):
		self.h = np.zeros([size,size])

	def addSinglePattern(self,p):
		#Update the hopfield matrix using the passed pattern
		lh=len(self.h)
		for i in range(0,lh):
			for j in range(i+1,lh):
				c=((2*p[i]-1)*(2*p[j]-1))
				self.h[i][j]=self.h[i][j]+c
				self.h[j][i]=self.h[j][i]+c

	def fit(self, patterns):
		# for each pattern
		# Use your addSinglePattern function to learn the final h matrix
		for i in range(0,len(patterns)):
			self.addSinglePattern(patterns[i])

	def retrieve(self, input):
		#Use your trained hopfield network to retrieve and return a pattern based on the
		#input pattern.
		#HopfieldNotes.pdf on canvas is a good reference for asynchronous updating which
		#has generally better convergence properties than synchronous updating.
		lh=len(self.h)
		e=[]
		for i in range(0,lh):
			r=0
			for j in range(0,lh):
				if i!=j:
					r+=input[j]*self.h[j][i]
			if r>=0:
				e.append(1)
			else:
				e.append(0)
		return e

	def classify(self, inputPattern):
		#Classify should consider the input and classify as either, five or two
		#You will call your retrieve function passing the input
		#Compare the returned pattern to the 'perfect' instances
		#return a string classification 'five', 'two' or 'unknown'

		c=self.retrieve(inputPattern)
		if c==two:
			return 'two'
		if c==five:
			return 'five'
		return 'unknown'




if __name__ == "__main__":
	hopfieldNet = HopfieldNetwork(25)
	hopfieldNet.fit(patterns)
	y=['five','two']
	mp=MLPClassifier()
	mpp=[MLPClassifier(hidden_layer_sizes=(100, 100,100,100,100,)),MLPClassifier(hidden_layer_sizes=(100,100,100,100,)),MLPClassifier(hidden_layer_sizes=(100,100,))]
	for i in range(0,len(mpp)):
		mpp[i]=mpp[i].fit(patterns,y)
	MLP=mp.fit(patterns,y)
	Label="label"
	Features=['r00','r01','r02','r03','r04','r10','r11','r12','r13','r14','r20','r21','r22','r23','r24','r30','r31','r32','r33','r34','r40','r41','r42','r43','r44']

	df=pd.read_csv("Luna-McBride-TrainingData.csv")
	df2=pd.read_csv("NewInput.csv")
	r=[]
	for column in df:
		r.append(column)
	df=df.rename(columns={r[0]:Features[0]})
	X, y=df[Features],df[Label]
	XX, yy=df[Features].append(df2[Features]),df[Label].append(df2[Label])
	XX.reset_index(inplace=True,drop=True)
	yy.reset_index(inplace=True,drop=True)

	w=0
	for i in range(0,len(X)):
		a=i
		c=X.loc[i]
		#utils.visualize(c)
		e=hopfieldNet.classify(c)
		if e==y.loc[i]:
			w+=1
		#print(e,y.loc[i])
	print(w/a)
	#e=hopfieldNet.classify([0,1,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1])
	#print(e,'two')
	#X.append([0,1,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1])
	q=MLP.predict(X)
	#print('\n')
	w=0
	for i in range(0,len(q)):
		a=i
		if q[i]==y.loc[i]:
			w+=1
		#print(q[i],y.loc[i])
	print(w/a)
	print('\n \n')

	cchold1=[]
	cchold2=[]
	cchold3=[]
	cchold4=[]
	cchold5=[]
	v=[]
	k=0
	while k<=0.5:
		fe=[]
		fn=[]
		cc=0
		for l in range(0,len(X)):
			fr=distort_input(X.loc[l],k)
			ce=hopfieldNet.classify(fr)
			if ce==y.loc[l]:
				cc+=1
			fe.append(fr)
		cchold1.append(cc/len(X))
		ff=MLP.predict(fe)
		w=0
		for l in range(0,len(ff)):
			if ff[l]==y.loc[l]:
				w+=1
		cchold2.append(w/len(ff))

		for j in range(0,len(XX)):
			fr=distort_input(XX.loc[j],k)
			fn.append(fr)
		ni=[]
		for i in range(0,len(mpp)):
			ni=mpp[i].predict(fn)
			w=0
			for j in range(0,len(ni)):
				if ni[j]==yy.loc[j]:
					w+=1
			if i==0:
				cchold3.append(w/len(ni))
			elif i==1:
				cchold4.append(w/len(ni))
			else:
				cchold5.append(w/len(ni))
		v.append(k)
		k+=0.01

	fig, ax=plt.subplots(figsize=(8,8))
	ax.plot(v,cchold1,'r')
	ax.plot(v,cchold2,'g')
	ax.plot(v,cchold3,'b')
	ax.plot(v,cchold4,'k')
	ax.plot(v,cchold5,'c')
	ax.set_xlabel('Distortion Rate',fontsize=16)
	ax.set_ylabel('Accuracy',fontsize=16)
	plt.show()

	#utils.visualize(five)
	#utils.visualize(two)


	#hopfieldNet.fit(patterns)
