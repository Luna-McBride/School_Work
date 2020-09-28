#!/usr/bin/env python
# coding: utf-8

# In[300]:


#Code by Luna McBride for Algorithms (CSCI 3104) Assignment 8B.
#Code was built in a Jupyter Notebook

import numpy as np #Numpy for array usage
import math #Math for the floor function, as we cannot have float indices
from astropy.table import Table, Column #Table libraries to, well, build the table we need to build


# In[369]:


#Randomize: A function to randomize an array for testing purposes
def randomize(n):
    num=np.array(range(1,n+1)) #Array of numbers, 1 to n (put in as n+1 as the range ends before the actual n)
    arr=np.random.choice(num, size=n,replace=False) #Array of randomized numbers, taking the num array in as to be randomized
    return arr #Return the array

#Partition: A function to divide the array and sort it for a value at the end. The backbone of a quicksort
def Partition(we,p,r):
    x=we[r] #Obtain the end value, which is used as the pivot
    i=p-1 #i value to keep track of where there is a value place to swap (greater than A[r])
    c=0 #A variable to keep count of conditionals
    
    #For loop to go through the array, sorting the values less than or equal to A[r] behind where A[r] will end up
    for j in range(p,r):
        if we[j]<=x: #If the value is lower than our pivot
            i+=1 #Shift the i up as the place to swap
            we[j],we[i]=we[i],we[j] #Swap this lesser value for the greater than value determined earlier by not entering here
            c+=1 #Add to count because conditional
       
    we[i+1],we[r]=we[r],we[i+1] #Swap the pivot into place
    return i+1,c #Return the index of the pivot as well as the count

#QuickSelect: A function to get the pivot we will use in QuickSort, as stated in the assignment
def QuickSelect(A,p,r,split):
    e=0 #The variable for conditional count
    if r<=p: #Checks if we are not looking at an array, but rather, a small subset of one or less items
        e+=1 #This is a conditional, so add to the count
        return p,e #Returns the count and the lower end as our index
    ind=math.floor(len(A)*split) #Obtains the kth smallest we are looking for given our split
    q,c=Partition(A,p,r) #Partition to split this into sides
    e+=c #Add partition's counts to the count variable
    if q==ind: #Checks if we have what we are looking for
        e+=1 #This is a conditional, so add to the count
        A[ind],A[r]=A[r],A[ind] #Swap the value to the end so it can act as the pivot
        return ind,e #Return our value with the count
    elif ind<q: #Checks if the partition is further than the desired value
        e+=2 #Add 2 to the count, as to count this conditional and the previous
        return QuickSelect(A,p,q-1,split),e #Recursively calls the function, gives that value, as well as the count
    else:
        e+=2 #Add only 2 to the count, as we are not actually comparing anything in our else
        return QuickSelect(A,q+1,r,split),e #Recursively calls the function, gives that value, as well as the count
    
#QuickSort: A function to sort our array given pivots from QuickSelect.    
def QuickSort(A,p,r,split):
    if p<r: #If we have more than one value
        c=1 #Count variable for the number of comparisons
        ind,e=QuickSelect(A,p,r,split) #Call to quickselect to get the pivot given our split value
        c+=e #Add count from quickselect to our count
        q,f=Partition(A,p,r) #Call partition to sort our pivot in our array
        c+=f #Add count from partition to our count
        a=QuickSort(A,p,q-1,split) #Recursive call to QuickSort as to follow the "Divide and Conquer" nature
        b=QuickSort(A,q+1,r,split) #Recursive call to QuickSort as to follow the "Divide and Conquer" nature
        if a==None: #If value to take care of the case it returns None, which does happen
            a=0 #None is essentially 0, so set it to 0
        if b==None: #If value to take care of the case it returns None, which does happen
            b=0 #None is essentially 0, so set it to 0
        if split==0.251:#I had set split to 0.251 on the call asking for printing the array at each step
            print(A) #Print the array after each recursive call
        return a+b+c #Return the count from all calls


# In[373]:


r=2 #Change around this to change the number of elements in the array

cond=np.zeros(10,dtype=int) #Array for the conditionals
nVal=np.zeros(10,dtype=int) #Array for the current n value
table=Table() #Call to create a table

#For loop to generate and test 10 randomized array
for i in range(1,11):
    n=r**i #Getting the 2^i the prompt wants
    nVal[i-1]=n #Holds the n for the table
    #arr=np.array(range(1,n+1)) #Creation of an array to see worst case
    arr=randomize(n) #Calling the randomize function to randomize the array (above)
    #print(arr) #A print to see the randomized array. Commented out as I am not currently testing it at the moment, but can be put back in
    cond[i-1]=QuickSort(arr,0,n-1,0.25) #The quicksort call to get counts and sort the array
    #print(c) #Print of the counts for analysis purposes
    #print(arr) #A print to see the sorted array. Uncommment if you want; its just sorted arrays

w=np.array([5,10,20]) #Values required for n for each recursive call

#For loop to go through the required values and get the arrays for each
for j in range(0,len(w)): 
    arr=randomize(w[j]) #Randomizes the array for the specific n value
    print(arr) #prints the array before
    c=QuickSort(arr,0,w[j]-1,0.251) #Quicksort call
    print(arr) #prints the array after
    
table["N"]=nVal #Set the first column of the table to the n values
table["Conditionals"]=cond #Set the second column of the table to the conditionals array
print(table) #Display the table


# In[ ]:




