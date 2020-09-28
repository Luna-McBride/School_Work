#!/usr/bin/env python
# coding: utf-8

# In[272]:


#coded by Luna McBride for the final of Algorithms, Question 5
import numpy as np


# In[273]:


#SetTar: a function to get the target value and what it is built from
def setTar(arr,t,k):
    if len(arr)==0: #if there is no array, there is not going to be a k
        return False,[] #return False because there is no k
    if len(arr)<k:#if there is not enough values
        return False,[] #return false as there is not k to fit the t
    T=np.zeros(shape=(len(arr)+1,t+1)) #setup a 2d array to build an array not based off k for math
    N=np.zeros(shape=(len(arr)+1,t+1)) #setup a 2d array to build the array based on k
    re=np.zeros(t+1) #an array to hold the point where trues in the array start
    
    #for loop to go through each value in the input, adding N to be mutiplied for the complexity
    for i in range(0,len(arr)+1):
        
        #for loop to go through each value up to target t (as dynamic programming goes), adding t to be multiplied in the complexity
        for j in range(0,t+1):
            if j==0: #base case for if we have a 0 target value
                T[i][j]=True #both are true, as
                N[i][j]=True #no numbers can add up to 0
            elif i==0: #if we consider no input value
                T[i][j]=False #These are all false, as
                N[i][j]=False #no input leads to no hits for the target value besides at 0,0
            elif (j-arr[i-1])>=0: #if we can fit the value in given our weights
                l=0 #set up a variable to count the k used to get here
                a=i #a variable to hop backwards in the array without messing up our actual current i,j
                b=j #a variable to hop backwards in the array without messing up our actual current i,j
                
                #while going unused, this loop was the basis that lead to the ideas in the for, so I shall leave it in in case I have it wrong and I can be told where my ideas are wrong
                #while a>0 and b>0:
                    #if N[a][b]==True:
                        #a=a-1
                    #else:
                        #b=b-arr[a-1]
                        #l+=1
                
                #For loop to look back in the array, utilizing the re array so we dont need to continuously hop up, eliminating the need for the while above and adding the k to the complexity
                for e in range(0,k):
                    if a<1 and b<1: #if we cannot hop back more
                        break #end loop
                    l+=1 #continuing loop, so count increases
                    w=re[b] #pull the value of re to get where there is somewhere we should jump to
                    w=int(w) #put it as an int, because it saves values as floats
                    if a>w-1: #if we are not already above the specified point
                        a=w-1 #set the a to our new point
                    b=b-arr[a-1] #set the b to utilize the new point
                if l==k and a<=0 and b<=0: #if we went through all the k and have definitely gone through the array
                    T[i][j]=T[i-1][j] or T[i-1][j-arr[i-1]] #set our t using the ideas from quiz 5
                    N[i][j]=T[i-1][j] or T[i-1][j-arr[i-1]] #let N use that too, as otherwise it would not correctly look through values and the array would end up as mostly 0's if relying on N along
                else: #if that is not the case
                    T[i][j]=T[i-1][j] or T[i-1][j-arr[i-1]] #still let T do the same thing as a basis
                    N[i][j]=N[i-1][j] #make N do the other case used below, as we are not considering this item for this case
                
            else: #if we cannot add it into our weights
                T[i][j]=T[i-1][j] #use the previous item,
                N[i][j]=N[i-1][j] #as it is possible for the program to have found something before now
            if T[i][j]==True and T[i-1][j]==False: #if we are at a point where we found a point where the target is obtained
                re[j]=i #get the item it ends with, which is used so we do not have to have a while to continuously go up without doing anything else
    
    c=[] #an array for holding the path back
    p=t #a variable to go down the target weights and get the end
    a=len(arr)-1 #a variable to go down the inputs to find the correct ones going backward
    
    #a while loop to go through and rebuild from the inputs, which is a + and not multipled for not being nested, and thus, not included in complexity
    while p>0 or a>0:
        if T[a][p]==True: #if we havent hit the point we know something was just chosen to fit t
            a=a-1 #move up the array without doing anything else, effectively ignoring the input
        else: #if we have found a False and thus something that has been used
            p=p-arr[a] #take the value from the weight
            c.append(arr[a]) #add the value to our list
    c.reverse() #reverse the values to fit them into an order as if we started at 0,0
    
    #print statements to look at the arrays. you can uncomment and see if you want
    #print(T)
    #print(N)
    #print(c)
    return N[len(arr)][t],c #return the final value and the c array


# In[274]:


t=6 #our target
k=2 #our k step length
l,r=setTar([2,3,4,5],t,k) #call to the function, producing the left output and right output (l,r)
if l==1: #if there is actually a value
    print("The subset {} has size {} with a target value of {}".format(r,k,t))
else: #if there is not
    print("No subset size {} adds up to {}".format(k,t))


# In[ ]:




