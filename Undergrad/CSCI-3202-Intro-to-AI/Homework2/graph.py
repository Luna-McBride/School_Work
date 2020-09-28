import random
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

fig,axes=plt.subplots(figsize=(8,8))
axes.set_xlabel("Generation", fontsize=20)
axes.set_ylabel("Average Fitness", fontsize=20)
axes.grid(alpha=0.25)
#for i in range(0,6):
gen=[]
aver=[]
p=random.randint(1,50)
e=pd.read_csv("run{}.txt".format(p),sep=" ",header=None)

for i in range(0,len(e)):
	c=e.loc[i]
	gen.append(c[0])
	aver.append(c[1])

plt.plot(gen,aver)

axes.set_title("run{}".format(p), fontsize=24)
plt.show()