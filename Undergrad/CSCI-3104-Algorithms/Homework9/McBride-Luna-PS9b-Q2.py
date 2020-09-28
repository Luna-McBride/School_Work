#!/usr/bin/env python
# coding: utf-8

# In[1]:


#Code by Luna McBride using Jupyter Notebook
import numpy as np


# In[94]:


#DP: a function to create a dynamic programming bottom-up approach
def dp(arr,n):
    point=[] #Number of points per number of assignments
    i=0 #Variable to use for our while loop to go through amounts of assignments
    
    #While loop to loop through our assignments to find the optimal number of points for each
    while i<n+1:
        hold=[] #Holder of the assignment chosen
        if i==0: #No need to compare if we are only looking at one value
            point.append(arr[0]) #Append the point value for one item
        else: #If we have a list bigger than 1
            summ=0 #Holder for the sum of all point values
            j=0 #Variable for the while loop to get the points for items 0 to i
            
            #While loop to find the optimal point values for assignments 1-i
            while j<i:
                if j==i-1: #If there is only one left
                    r=arr[i-1] #Get that last value
                    hold.append(arr[i-1]) #Put the value into the array of chosen items
                else: #If there are more than one value left to check
                    r=max(arr[j],arr[j+1]) #Find the max of the two neighboring items
                    if r==arr[j+1]: #If it is the right item
                        j=j+1 #Add an extra to j, eliminating the possibility of having two consecutive assignments
                    hold.append(arr[j]) #Append the chosen value to our list of chosen values
                summ+=r #Add the value to our sum
                j+=2 #Jump j up by 2, also to try to eliminate consecutive homeworks
            point.append(summ) #add this point sum to our list
        i+=1 #Up i by 1, expanding our scope by one assignment
    return point[n],hold #Return our final points as well as our chosen values of the last iteration
            

#Assignment: a function to do the dynamic programming using only Big Theta 1 space
def assignment(arr,n):
    summ=0 #Our holder for the best sum
    e=0 #A variable to iterate through our array of assignments (arr)
    
    #While loop to loop through our values and pick the best assignments
    while e<n:
        if e==n-1: #if there is only one value left to check
            r=arr[n-1] #add the value to our list
        else: #If there are more than two left to check
            r=max(arr[e],arr[e+1]) #Pick the best between a value and its neighbor
            if r==arr[e+1]: #If the one to the right was best
                e=e+1 #Add to our iterator one, skipping the possibility of choosing the value after this one
        summ+=r #Build our sum from the best
        e+=2 #Add 2 to the iterator, jumping by two to try to ignore sequential assignments
    return summ #Return the points of the best combination

#Greedy: a function to implement the greedy max(sum(even),sum(odd)) given in the homework
def greedy(arr,n):
    even=[] #An array for our even values
    odd=[] #An array for our odd values
    
    #For loop to put the odd values in the odd list and vice versa
    for i in range(0,n):
        if i%2==1: #If the index is odd
            odd.append(arr[i]) #Add value to odd
        else: #If the index is even
            even.append(arr[i]) #Add value to even
    m=max(sum(even),sum(odd)) #Greedy function given by homework
    if m==sum(even): #If the even was the best
        return m,even #Return the best value with the evens, the best list
    return m,odd #Return the best value with the odds, the best list


# In[95]:


arr=np.array([20,6,13,100,11,2])
print("Array:")
print(arr)
n=len(arr)

c,d=dp(arr,n)
print("Optimal:")
print(d)
print(c)

print("Optimal Theta(1)")
print(assignment(arr,n))

a,b=greedy(arr,n)
print("Greedy:")
print(b)
print(a)


# In[ ]:





# In[ ]:





# In[ ]:




