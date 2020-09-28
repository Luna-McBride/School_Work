#!/usr/bin/env python
# coding: utf-8

# In[492]:


#Code by Luna McBride for the Algorithms final exam question 3
import numpy as np


# In[493]:


#Epic: a function to do a single non-greedy turn of Epic!
#This uses ideas from dynamic programming, but does not go too deep as we cannot actually tell much after that point what choices will be made
def epic(A,p,r):
    if p==r: #if there is only one
        return A[p] #return the one
    if r-p==1:#if there are exactly 2 values, as p is always lower than r
        return min(A[p],A[r]) #return the smallest
    
    #All of these use comparisons with both the ends and which would be the next potential ends, in order to look ahead,
    #Connecting P and R-1 (the one that would still be blocked and the one that would be taken) and R and P+1 for the same reason
    if (A[p]+A[r-1])>(A[r]+A[p+1]) and A[p]>A[r]: #if the r connection and r is smaller
        return A[r] #return the right edge
    if (A[p]+A[r-1])<(A[r]+A[p+1]) and A[p]<A[r]: #if the p (left) connection and p are smaller
        return A[p] #return the left edge
    if (A[p]+A[r-1])<(A[r]+A[p+1]): #if the p connection is smaller but p is not, making the r-1 something we do not want the greedy to take 
        return A[p] #return the left to preserve that right
    return A[r] #return the right to preserve the left for the same reason as the if above but with r and p+1

    #Another idea I was testing, left commented because I want to look at it more after there is no longer a deadline
    #if (A[p]+A[r-1])>(A[r]+A[p+1]) and (A[p]-A[r-1])<2 and (A[p]-A[r-1])>-2:
        #return A[r]
    #if A[p]+A[r-1]<A[r]+A[p+1] and A[r]-A[p+1]<2 and A[r]-A[p+1]>-2:
        #return A[p]
    #else:
        #return min(A[p],A[r])


# In[494]:


#Greedy: an algorithm to implement the greedy function noted in the question
def greedy(A,p,r):
    return min(A[p],A[r]) #pick the smallest of your choices


# In[495]:


#Play: A function to actually play the game. Returns the scores of the players p1 and p2
def play(arr):
    a=0 #Variable representing the beginning of the array (card line)
    b=len(arr)-1 #variable representing the end of the array
    r=len(arr) #variable representing the number of cards picked, used for our while loop
    c=0 #a sort of switch, 0 for player 1's turn, 1 for player 2's.
    p1=0 #The sum of player 1's scores, being our non-greedy function epic
    p2=0 #The sum of player 2's scores, being the greedy algorithm

    #A while loop to go through all of our cards to determine score. Being the only loop, this effectively makes this algorithm overall O(r)
    while r>0:
        if c==0: #If it is player 1's turn
            ni=epic(arr,a,b) #do the optimal strategy
            p1+=ni #add to the score
            if ni==arr[a]: #If this was pulled from the left
                a+=1 #move the left one forward to create a new left
            else: #If it was pulled from the right
                b-=1 #move the right of the array over and do not consider that drawn card
            c=1 #make it player 2's turn
        else:#If it is player 2's turn
            ni=greedy(arr,a,b)#do the greedy strategy
            p2+=ni#add to the score
            if ni==arr[a]:#If this was pulled from the left
                a+=1#move the left one forward to create a new left
            else:#If it was pulled from the right
                b-=1#move the right of the array over and do not consider that drawn card
            c=0#make it player 1's turn again
        r-=1 #remove one card from consideration
    
    return p1,p2


# In[498]:


arr=np.array(range(1,11)) #Get an array of values 1 to 10
arr=np.random.choice(arr,size=len(arr),replace=False) #randomize those values in the array
arr=[8,4,9,10,3,6,2,7,1,5]

print(arr) #print the array for our sake
print(play(arr)) #print the scores


# In[502]:


a=np.array(range(1,101)) #Get an array of values 1 to 100
arr=np.random.choice(a,size=len(a),replace=False) #randomize those values in the array

p1,p2=play(arr)

print("Greedy Score: {}".format(p2))
print("Non-greedy Score: {}".format(p1))


# In[ ]:




