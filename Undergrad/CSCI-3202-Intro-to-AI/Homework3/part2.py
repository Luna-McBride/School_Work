import pandas as pd
import numpy as np
import pickle
import matplotlib.pyplot as plt
from sklearn import preprocessing
from sklearn import metrics
from sklearn import model_selection
from sklearn.linear_model import LogisticRegression
from sklearn.naive_bayes import GaussianNB
from sklearn.svm import SVC
from sklearn.gaussian_process import GaussianProcessClassifier
from sklearn import tree
from sklearn.neighbors import KNeighborsClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import GridSearchCV
le = preprocessing.LabelEncoder()

#Helpful: https://scikit-learn.org/stable/auto_examples/model_selection/plot_roc_crossval.html#sphx-glr-auto-examples-model-selection-plot-roc-crossval-py
#https://scikit-learn.org/stable/auto_examples/model_selection/plot_roc.html#sphx-glr-auto-examples-model-selection-plot-roc-py

gridS=[{'C':[0.1,1,100],'gamma':[1e-3,1e-4,0.1]}]
gridTree=[{'max_depth':[0.5,30,100], 'n_estimators':[1,20,200]}]
classifiers = {"Logistic Regression": LogisticRegression(solver='lbfgs'), "Naive Bayes": GaussianNB(), "SVM": SVC(probability=True,gamma='auto'), "KNeighbor": KNeighborsClassifier(),
			   "Decision Tree": tree.DecisionTreeClassifier(), "Random Forest": RandomForestClassifier(), "Gaussian Classifier": GaussianProcessClassifier()
			   }


Label = "Credit"
Features = ["A1","A2","A3","A4","A5","A6","A7","A8","A9","A10","A11","A12","A13","A14","A15","A16","A17","A18","A19"]
le.fit(Features)

def saveBestModel(clf):
	pickle.dump(clf, open("bestModel.model", 'wb'))

def readData(file):
	df = pd.read_csv(file)
	return df

def trainOnAllData(df, clf):
	print("TODO")
		


	saveBestModel(clf)

df = readData("credit_train.csv")
X, y = df[Features], df[Label]
X = StandardScaler().fit_transform(X)
figure = plt.figure(figsize=(24, 6))

k=10
fold=model_selection.KFold(n_splits=k)
i=1
for name, clf in zip(classifiers.keys(), classifiers.values()):
	tprs=[]
	meanfpr=np.linspace(0,1,100)
	auc=[]
	e=""
	if name =="Random Forest":
		grid=GridSearchCV(clf,gridTree,cv=5,scoring='accuracy')
	if name=="SMV":
		grid=GridSearchCV(clf,gridS,cv=5,scoring='accuracy')
	for train,test in fold.split(X,y):
		if name=="Random Forest" or name=="SMV":
			grid.fit(X[train],y[train])
			fi=grid
		else:
			fi=clf.fit(X[train],y[train])
		ax = plt.subplot(1, len(classifiers)+1,i)
		pro=fi.predict_proba(X[test])
		fpr,tpr,thre=metrics.roc_curve(y[test],pro[:,1],pos_label="good")
		tprs.append(np.interp(meanfpr,fpr,tpr))
		tprs[-1][0]=0.0
		au=metrics.auc(fpr,tpr)
		auc.append(au)
	i+=1
	plt.plot([0,1],[0,1],color='g')
	mean=np.mean(tprs,axis=0)
	mean[-1]=1.0
	meaau=metrics.auc(meanfpr,mean)
	std=np.std(auc)
	plt.xlabel('False Positive Rate')
	plt.ylabel('True Positive Rate')
	ax.plot(meanfpr,mean,color='r',label='Mean (AUC= %0.2f $\pm$ %0.2f)' % (meaau,std))
	ax.set_title(name, fontsize=20)
	plt.legend(loc="lower right")
	if name=="SVM":
		saveBestModel(clf)
figure.subplots_adjust(left=.02, right=1.1)
plt.xlim([0.0, 1.0])
plt.ylim([0.0, 1.0])
plt.show()

