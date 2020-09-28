#!/usr/bin/env python
# coding: utf-8

# In[220]:


import numpy as np
import math
from astropy.table import Table, Column


# In[21]:


def randomize(n):
    num=np.array(range(1,n+1))
    arr=np.random.choice(num,size=n,replace=False)
    return arr

def numflips(n,arr):
    if n<0:
        print("Please give an n greater than 0")
        
    flipCount=0
    
    p=n
    if n%2==1:
        p=n-1
    
    for i in range(0,p,2):
        item=arr[i]
        item2=arr[i+1]
        
        for ii in range(i+1,n):
            select=arr[ii]
            
            if item>select:
                flipCount+=1
            if item2>select:
                flipCount+=1
    return flipCount


# In[213]:


def Merge(A,p,q,r):
    i=0
    j=0
    k=p
    
    x=q-p+1
    y=r-q
    l=np.zeros(x)
    r=np.zeros(y)
    
    flip=0
    
    for w in range(0,x):
        l[w]=A[p+w]
    for z in range(0,y):
        r[z]=A[q+1+z]
    
    while i<x and j<y:
        if l[i]<=r[j]:
            A[k]=l[i]
            i+=1
        else:
            for v in range(i,x):
                if l[v]>r[j]:
                    flip+=1
            A[k]=r[j]
            j+=1
        k+=1
       
    while i<x:
        A[k]=l[i]
        i+=1
        k+=1
    while j<y:
        A[k]=r[j]
        j+=1
        k+=1
    return flip,A
            
def MergeSort(A,p,r):
    lCount=0
    rCount=0
    q=math.floor((p+r)/2)
    e=0
    if p<r:
        lCount=MergeSort(A,p,q)
        rCount=MergeSort(A,q+1,r)
        e,ce=Merge(A,p,q,r)
        A=ce
        return e+lCount+rCount
    return 0


# In[221]:


flipA=np.zeros(12,dtype=int)
flipM=np.zeros(12,dtype=int)
nVal=np.zeros(12,dtype=int)
table=Table()

for qwerty in range(1,13):
    pu=randomize(2**qwerty)
    nVal[qwerty-1]=2**qwerty
    flipA[qwerty-1]=numflips(2**qwerty,pu)
    flipM[qwerty-1]=MergeSort(pu,0,(2**qwerty)-1)

table["N"]=nVal
table["Flips"]=flipA
table["MergeFlips"]=flipM
print(table)

