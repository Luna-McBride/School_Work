#!/usr/bin/env python
# coding: utf-8

# In[3]:


import numpy as np
import math


# In[177]:


def part(h,p,r):
    l=len(h)-1
    c=0
    x=h[r]
    i=p-1
    for j in range(p,r):
        if h[j]<=x:
            i+=1
            h[j],h[i]=h[i],h[j]
        else:
            if h[j]>j and l==r and p==0:
                c+=1
    h[i+1],h[r]=h[r],h[i+1]
    return i+1,c
    
def hindex(h,p,r):
    c=0
    if p<r:
        q,c=part(h,p,r)
        a=hindex(h,p,q-1)
        b=hindex(h,q+1,r)
        if a==None:
            a=0
        if b==None:
            b=0
        return a+b+c


# In[178]:


#This is used to test the hindex. If you wish to do different tests, comment this out
for i in range(0,5):
    num=np.array(range(0,10))
    h=np.random.choice(num,size=10)
    h[::-1].sort()
    print(h)
    o=hindex(h,0,9)
    print(o)


# In[ ]:




