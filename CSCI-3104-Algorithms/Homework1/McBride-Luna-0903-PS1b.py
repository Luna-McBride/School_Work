#!/usr/bin/env python
# coding: utf-8

# In[55]:


#Made in Jupyter Notebook, coded by Luna McBride

import numpy as np
from astropy.table import Table, Column #https://docs.astropy.org/en/stable/table/construct_table.html


# In[56]:


#A function to count the number of flips in a randomized array sized n
def num_flips(n):
    if n<=0: #Simple check for the n>=1 condition specified in the prompt
        print("n must be more than 0")
       
    flipCounter=0 #counter for the number of flips
    numbers=np.array(range(1,n+1)) #An array holding the list of numbers 1-n in order to be randomized
    array=np.random.choice(numbers,size=n,replace=False) #The randomized array
    
    #A failsave for odd numbers n to not compete with my loop unrolling
    p=n 
    if n%2==1:
        p=n-1 #p is n if even, but needs to be decrimented to not go over the array size. There is no possibility to get flips with the last number anyway
    
    #For loop to get the number of flips
    for i in range(0,p,2):
        item=array[i] #holder of the current tested value
        itemTwo=array[i+1] #holder for the next value following loop unrolling
        
        #For loop to test for flips throughout the rest of the array
        for ii in range(i+1,n):
            secItem=array[ii] #The second item, which is to be tested for flips against the current value
            if item>secItem: #Test for flips for the first item
                flipCounter+=1
            if itemTwo>secItem: #Test for flips with the other item brought in from unrolling
                flipCounter+=1
    
    return flipCounter #return the count of flips
 
flipArr=np.zeros(12,dtype=int) #Array to hold the number of flips
nVal=np.zeros(12,dtype=int) #Array to hold the n values
table=Table() #Table variable following the above sourced material

#For loop to get the n values and flips in accordance with the prompt
for i in range(1,13):
    nVal[i-1]=2**i #Calculator for the n value
    flipArr[i-1]=num_flips(int(nVal[i-1])) #Calls the flip function with the n value
    
table["N"]=nVal #Setup for the n row of the table
table["Flips"]=flipArr #Construction of the flips row of the table
print(table) #Display the table

