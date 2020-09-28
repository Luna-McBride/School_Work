#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np


# In[54]:


#Fix compared to my 7a:
# Swap strategy Source: https://stackoverflow.com/questions/22847410/swap-two-values-in-a-numpy-array
# Added a loop to get the B[r] equal to the A[r], which does not add to worst case but makes it actually work

def Partition(A,B,p,r):
    x=A[r]
    i=p-1
    
    for e in range(p,r):
        if B[e]==A[r]:
            B[r],B[e]=B[e],B[r]
    for j in range(p,r):
        if B[j]<=x:
            i+=1
            B[j],B[i]=B[i],B[j]
    B[i+1],B[r]=B[r],B[i+1]
    
    y=B[i+1]
    ni=p-1
    
    for ew in range(p,r):
        if A[ew]<y:
            ni+=1
            A[ew],A[ni]=A[ni],A[ew]
    A[ni+1],A[r]=A[r],A[ni+1]
    return i+1

def QuickSort(A,B,p,r):
    if p<r:
        q=Partition(A,B,p,r)
        QuickSort(A,B,p,q-1)
        QuickSort(A,B,q+1,r)


# In[56]:


#Form: Both unsorted, both sorted
#Used to test for correctness. Comment this out if you want to do your own tests
for i in range(0,10):
    a=100
    num=np.array(range(0,a))
    arr1=np.random.choice(num,size=a,replace=False)
    arr2=np.random.choice(num,size=a,replace=False)
    print("Unsorted {}:".format(i))
    print(arr1)
    print(arr2)
    QuickSort(arr1,arr2,0,a-1)
    print("Sorted {}:".format(i))
    print(arr1)
    print(arr2)


# In[ ]:




