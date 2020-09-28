# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print("Start:", problem.getStartState())
    print("Is the start a goal?", problem.isGoalState(problem.getStartState()))
    print("Start's successors:", problem.getSuccessors(problem.getStartState()))
    """
    start=problem.getStartState() #Retrieve the start state
    current=util.Stack() #Create a stack for the current values
    current.push((start,[])) #Initialize the current items
    check=[] #Holds already used items
    isGoal=0 #Pseudo-boolean checking if we are at the goal
    p=[] #Holder for the final list

    #While loop to search through our states
    while isGoal==0:
    	eck=set(check) #Turns the list of already used items into a set so I can go "_ not in _"
    	kek=current.pop() #Collects the next value
    	if problem.isGoalState(kek[0]): #CHecks if we are at the end
    		isGoal=1 #Lets the loop end
    		p=kek[1] #Gives the final path to the final list holder
    	else: #Not at the goal yet
    		succ=problem.getSuccessors(kek[0]) #Gets the successors to the current value

    		#For loop to check through each possible successor
    		for i in range(0,len(succ)):
    			if succ[i][0] not in eck: #Checks if the successor is already checked
    				w=succ[i][0] #Holds the value of the successor coordinates
    				current.push((w,kek[1]+[succ[i][1]])) #Pushes the sucessor to be checked
    				check.append(kek[0]) #Adds the successor to the list of checked nodes

    return p
    

def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    isGoal=0 #boolean-like item specifically for breaking the while loop once the goal is found
    p=[] #Holder for the final 
    start=problem.getStartState() #obtaining the start
    current=util.Queue()
    current.push((start,[])) #A variable for the current state
    ar=[start] #Set of all values checked specifically in the coordinates form
    c=util.Stack() #Holder for the order of directions
    layer=0
    
    #While loop to loop through the ways to go until we have the end
    while isGoal==0:
    	eck=set(ar) #Putting the already chose values in a set so I can do "if _ in ar" without a loop
    	reg=current.pop()
    	wq=problem.isGoalState(reg[0])
    	if wq==1: #Checks if we are in the goal state
    		isGoal=1 #sets so we can break the loop
    		p=reg[1]
 
    	#If it is not the goal node
    	else:
    		succ=problem.getSuccessors(reg[0]) #Get the successors from the current node

    		#For loop to go through the successors and add them to the queue
    		for i in range(0,len(succ)):
    			ree=reg[1] #Holder for the current list of nodes visited
    			if succ[i][0] in eck: #Checks if we have already been through this node
    				layer=layer #Placeholder line so it will not do the stuff to get added
    			else: #If not already checked
    				w=succ[i][0] #Puts a variable w to be the coordinate
    				current.push((w,ree+[succ[i][1]]))
    				ar.append(succ[i][0]) #Adds the coordinates to the checked coordinates list
    return p #Returns the list of directions
    

def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    isGoal=0 #Pseudo-boolean to see if it is the goal state
    start=problem.getStartState() #Obtain the start node
    current=util.PriorityQueue() #Queue to obtain the nexr value, just with the benefit of comparing weights
    current.push((start,[]),0) #Push the start state and blank weights and path
    check=[] #list of already traveled-to vertices
    p=[] #Holder for the final path

    #While loop to keep calculating until the goal is found
    while isGoal==0:
    	kek=current.pop() #Get the next value so we can use it
    	if problem.isGoalState(kek[0])==1: #Checks if we are at the end
    		isGoal=1 #Notify that we have hit the end point
    		p=kek[1] #Give the holder the final route
    	else: #We are not at the goal
    		if not kek[0] in check: #Checks if we have seen this value before
    			check.append(kek[0]) #Puts it in the list of checked values
    			succ=problem.getSuccessors(kek[0]) #Gets all the next sections we can go to

    			#For loop to get info on every successor and put it into the queue
    			for i in range(0,len(succ)):
    				w=succ[i][0] #Holder for the coordinates of the next point
    				he=kek[1]+[succ[i][1]] #Holder for the path we are going down currently
    				current.push((w,he),problem.getCostOfActions(he)) #Puts the next one in the cue, making sure to keep track of overall cost
    return p #Returns the final route


def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""
    start=problem.getStartState() #Obtains the start value
    current=util.PriorityQueue() #Makes a queue for the next items, but with respect to weights
    current.push((start,[]),heuristic(start,problem)) #Insert the initial state into the queue
    check=[] #Our list of values we already checked
    p=[] #Holder for our final route
    isGoal=0 #Defacto variable to check if we are done

    #Loops through our routes until we get to the end
    while isGoal==0:
    	kek=current.pop() #Pull out the next node to check
    	if problem.isGoalState(kek[0])==1: #Are we at the end?
    		isGoal=1 #Say we are at the end
    		p=kek[1] #Hold the final route to return
    	else: #No, we are not at the end
    		if not kek[0] in check: #Checks if we have seen this spot before
    			check.append(kek[0]) #Adds to the list we have seen before
    			succ=problem.getSuccessors(kek[0]) #Get the successors

    			#For loop to loop through the successors and put them in the queue
    			for i in range(0,len(succ)):
    				w=succ[i][0] #Holder for the coordinates of this next spot
    				he=kek[1]+[succ[i][1]] #Holder for the path, including this next step
    				
    				#Puts the path in, including keeping track of weights and the heuristic
    				current.push((w,he),heuristic(w,problem)+problem.getCostOfActions(he))


    return p #Returns the final path

# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
