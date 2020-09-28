#!/usr/bin/env python
# coding: utf-8

# In[1]:


#Coded by Luna McBride in a Jupyter Notebook

import numpy as np #Used for easier array use
from astropy.table import Table, Column #Used to build a table of similar strings


# In[2]:


#alignStrings: uses dynamic programming to build a 2D cost array to turn one string into another
def alignStrings(x,y,ins,de,sub):
    S=np.zeros(shape=(len(x)+1,len(y)+1)) #intialize the 2d array for dynamic programming
    
    #For loop to get the horizontal values j in respect to the inner loop for i
    for j in range(0,len(y)+1):
        
        #For loop to get the vertical values i in the array in respect to the outer loop for j (i,j are needed for a 2d array)
        for i in range(0,len(x)+1):
            if j==0 and i==0: #If we are at the origin, being blank strings
                S[i][j]=0 #There is nothing built yet, so it is 0
            elif j==0: #if our horizontal is at 0
                S[i][j]=S[i-1][j]+de #set cost in respect to delete, as delete cost is based on the vertical
            elif i==0: #if the vertical is at 0
                S[i][j]=S[i][j-1]+ins #set cost in respect to insert, as insert cost is used for horizontal movement
            elif x[i-1]==y[j-1]: #If our letters are the exact same
                S[i][j]=S[i-1][j-1] #take the previous diagonal, as same letters do not add to cost and we do not need to consider del or ins
            else: #If we are not in these other base cases
                S[i][j]=min(S[i-1][j-1]+sub,S[i][j-1]+ins,S[i-1][j]+de) #Pick the lowest of the adjacent values plus the cost to get to i,j
    return S #return the 2d array

#extractAlignment: takes the dynamic 2d array and gets the operations to get there
def extractAlignment(S,x,y,ins,de,sub):
    a=[] #the holder array for all of our directions
    i=len(x) #the horizontal range in our program
    j=len(y) #the vertical range of the program, together with i making the best cost overall
    prev=S[i][j] #Set the previous value to the end of the array
    
    #While loop to go from the end to the beginning of the array
    while i>0 or j>0:
        c=min(S[i-1][j-1],S[i-1][j],S[i][j-1]) #Pick the minimum of the previous values for best cost
        if c==S[i-1][j-1]: #If diagonal, and thus the best is a sub
            if c==prev: #if the cost does not change
                a.append('no-op') #insert a no-op, as cost did not change
            else: #If cost changes
                a.append('sub') #insert a sub value, as we had to substitute to get here
            i=i-1 #set our i
            j=j-1 #set our j, as both i and j need to be updated for a sub
        elif c==S[i-1][j]: #if vertical, thus being a delete
            if c==prev: #if the cost does not change
                a.append('no-op')#insert a no-op, as cost did not change
            else: #if cost changes
                a.append('delete') #insert a delete, as something needs to be deleted to do this
            i=i-1 #set our i, which is the only one considered in a delete
        else: #If horizontal, our last case
            if c==prev: #if the cost does not change
                a.append('no-op') #insert a no-op, as cost did not change
            else: #if cost did change
                a.append('insert') #insert an insert, as something must be added to reach this point
            j=j-1 #set the j, which is the only one considered in an insert scenario
        prev=c #set the previous to the current, which is now the previous because this is done
    a.reverse() #reverse the set so it is in an order us humans expect
    return a #return the list of operations

#commonSubstrings: uses the no-ops from the list a to see what stays the same between strings
def commonSubstrings(x,L,a):
    count=[] #array to hold a single list longer than our L
    hold=[] #holder for the longest string
    item=[] #holds the whole list of common strings
    items="" #conveter of a list of letters into the string
    it=[] #holder for the lengths of the common strings
    c=0 #current length of string
    cwin=0 #length of the longest string up until now
    
    #For loop to loop through the length of our operations for no-op values
    for i in range(0,len(x)+1):
        if a[i]=='no-op': #if we have a no-op
            count.append(x[i-1]) #put the letter into our list
        else: #if this was not a no-op
            if c>=L: #if we reached our L to track
                
                #For loop added specifically in the 'plagarism' part c to get the string for the table
                for j in range(0,len(count)):
                    items=items+count[j] #turn this from an array into a string
                item.append(items) #append the string to our item list for the table
                it.append(c) #add the count to the array of counts for the table
            count=[] #reset the array for our current iteration
            c=0 #set the current count back to 0
        if len(count)==L: #if we have reached our L
            c=L #set the current c to L to start tracking
            if c>cwin: #if this is our overall best
                cwin=c #set the overall best variable
                hold=count #hold onto the current iteration
        elif len(count)>=L: #if we have continued past our L
            c+=1 #increase our current count
            if c>cwin: #if this is our new overall best
                cwin=c #set the overall best to the new value
                hold=count #hold the current list thus far
                
    table=Table() #initialize the table
    table["length"]=it #set the lengths column of our table
    table["substring"]=item #set the substrings into our table
    return hold,cwin, table #return the overall best, its count, and the table


# In[3]:


x="exponential" #test x value to compare to and create our test y value
y="polynomial" #test y value
ins=2 #the cost to insert
de=1 #the cost to delete
sub=2 #the cost to substitute

S=alignStrings(x,y,ins,de,sub) #get our dynamic 2d array
print(S) #print our array
a=extractAlignment(S,x,y,ins,de,sub) #get the list of actions from our 2d array
print(a) #print our actions
c,d,e=commonSubstrings(x,2,a) #get the common strings to test operations
print(c,d) #print the overall longest


# In[5]:


song1=[] #array to put song into
f=open("Song1_Folsom_Prison.txt",'r') #open song 1's file
for line in f: #read through each line
    song1.append(line) #put the line in the array

song2=[] #array to put song 2 into
g=open("Song2_Crescent_City_Blues.txt",'r') #open the song 2 file
for line in g: #read through each line
    song2.append(line) #add the line to our array
f.close()
g.close()
    
x=song1[0] #put the song into a string variable, as it is all one line
y=song2[0] #put song 2 into a string variable
S=alignStrings(x,y,1,1,1) #get the dynamic array for the songs
a=extractAlignment(S,x,y,1,1,1) #get the operations for the songs
c,d,e=commonSubstrings(x,10,a) #do common substrings for song similarities
e.pprint() #print the table that hides the substring section because it is super long
e["substring"].pprint()#print the substring column specifically because it is huge


# In[ ]:





# In[ ]:




