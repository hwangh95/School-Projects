# multiAgents.py
# --------------
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


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent

class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]


        "*** YOUR CODE HERE ***"
        if successorGameState.isWin():
            return float("inf")
        #hacky way of preventing a min() empty sequence error        
        if successorGameState.isLose():
            return -float("inf")
        #any action that causes a loss should be weighted negative infinity

        closestFood = min([util.manhattanDistance(newPos,x) for x in newFood.asList()])
        #just goes through all the foods and gives the distance for the closest one
        numFoodLeft = len(newFood.asList())
        closestGhost = min([util.manhattanDistance(newPos,x.getPosition()) for x in newGhostStates])
        #same for the ghosts

        score  = successorGameState.getScore()-2*closestFood-5*(1./closestGhost)-25*numFoodLeft
        #the pacman should try to reduce the distance to the closest food but it should prioritize minimizing the pellets. the closestGhost is inversed because we want to increase the distance to the ghosts as much as possible

        #the actual coefficients were a result of trial and error
        return score


def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.

          Here are some method calls that might be useful when implementing minimax.

          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1

          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action

          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"

        def maxValue(gameState, depth):
            if gameState.isWin() or gameState.isLose() or depth == 0:
                return (self.evaluationFunction(gameState),"")
            v = -float("inf")
            bestScore = v
            bestAction = ""
            legalActions = gameState.getLegalActions(0)
            for a in legalActions:
                successorState = gameState.generateSuccessor(0,a)
                v = max(v, minValue(successorState, depth,1)[0])
                if v > bestScore:
                    bestScore = v
                    bestAction = a
            return (bestScore,bestAction)

        
        def minValue(gameState, depth, agentIndex):
            if gameState.isWin() or gameState.isLose() or depth == 0:
                return (self.evaluationFunction(gameState),"")
            v = float("inf")
            bestScore = v
            bestAction = ""
            legalActions = gameState.getLegalActions(agentIndex)
            for a in legalActions:
                successorState = gameState.generateSuccessor(agentIndex,a)
                if agentIndex != (gameState.getNumAgents()-1):
                    v = min(v, minValue(successorState, depth, agentIndex+1)[0])
                    if v < bestScore:
                        bestScore = v
                        bestAction = a
                else:
                    v = min(v, maxValue(successorState, depth-1)[0])
                    if v < bestScore:
                        bestScore = v
                        bestAction = a
            return (bestScore,bestAction) 
        
        #The maxValue and minValue are just implementations of the maxValue minValue functions given in the lecture slides
        #It's been modified to include multiple agents and to keep track of the best action along with the best score

        return maxValue(gameState,self.depth)[1]

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"

        def maxValue(gameState, depth, alpha, beta):
            if gameState.isWin() or gameState.isLose() or depth == 0:
                return (self.evaluationFunction(gameState),"")
            v = -float("inf")
            bestScore = v
            bestAction = ""
            legalActions = gameState.getLegalActions(0)
            for a in legalActions:
                successorState = gameState.generateSuccessor(0,a)
                v = max(v, minValue(successorState, depth,1,alpha,beta)[0])
                if v > bestScore:
                    bestScore = v
                    bestAction = a
                if v > beta:
                    return (bestScore,bestAction)
                alpha = max(alpha,v)

            return (bestScore,bestAction)

        
        def minValue(gameState, depth, agentIndex, alpha, beta):
            if gameState.isWin() or gameState.isLose() or depth == 0:
                return (self.evaluationFunction(gameState),"")
            v = float("inf")
            bestScore = v
            bestAction = ""
            legalActions = gameState.getLegalActions(agentIndex)
            for a in legalActions:
                successorState = gameState.generateSuccessor(agentIndex,a)
                if agentIndex != (gameState.getNumAgents()-1):
                    v = min(v, minValue(successorState, depth, agentIndex+1,alpha,beta)[0])
                    if v < bestScore:
                        bestScore = v
                        bestAction = a
                    if v<alpha:
                        return (bestScore,bestAction)
                    beta = min(beta,v)
                else:
                    v = min(v, maxValue(successorState, depth-1,alpha,beta)[0])
                    if v < bestScore:
                        bestScore = v
                        bestAction = a
                    if v<alpha:
                        return (bestScore,bestAction)
                    beta = min(beta,v)
            return (bestScore,bestAction) 

        #The above code is the minimax algorithm whith the alpha-beta checks added at every recursion point.
        return maxValue(gameState,self.depth,-float("inf"),float("inf"))[1]

class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
          Returns the expectimax action using self.depth and self.evaluationFunction

          All ghosts should be modeled as choosing uniformly at random from their
          legal moves.
        """
        "*** YOUR CODE HERE ***"
        def maxValue(gameState, depth):
            if gameState.isWin() or gameState.isLose() or depth == 0:
                return (self.evaluationFunction(gameState),"")
            v = -float("inf")
            bestScore = v
            bestAction = ""
            legalActions = gameState.getLegalActions(0)
            for a in legalActions:
                successorState = gameState.generateSuccessor(0,a)
                v = max(v, expValue(successorState, depth,1)[0])
                if v > bestScore:
                    bestScore = v
                    bestAction = a
            return (bestScore,bestAction)

        
        def expValue(gameState, depth, agentIndex):
            if gameState.isWin() or gameState.isLose() or depth == 0:
                return (self.evaluationFunction(gameState),"")
            v = 0 
            legalActions = gameState.getLegalActions(agentIndex)
            for a in legalActions:
                successorState = gameState.generateSuccessor(agentIndex,a)
                if agentIndex != (gameState.getNumAgents()-1):
                    v += expValue(successorState, depth, agentIndex+1)[0]
                else:
                    v += maxValue(successorState, depth-1)[0]
            return ((v+0.0)/float(len(legalActions)),"") 

        #Since the probabilities are uniform across legalActions, I just set the expectation as the summed values divided by the number of legal actions.
        return maxValue(gameState,self.depth)[1]

def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).

      DESCRIPTION: <write something here so we know what you did>
    """
    "*** YOUR CODE HERE ***"
    pos = currentGameState.getPacmanPosition()
    food = currentGameState.getFood()
    ghostStates = currentGameState.getGhostStates()
    normGhosts = []
    scaredGhosts = []
    for g in ghostStates:
        if g.scaredTimer:
            scaredGhosts.append(g)
        else:
            normGhosts.append(g)

    if currentGameState.isWin():
        return float("inf")
    #hacky way of preventing a min() empty sequence error        
    if currentGameState.isLose():
        return -float("inf")
    #any action that causes a loss should be weighted negative infinity

    closestFood = min([util.manhattanDistance(pos,x) for x in food.asList()])
    #just goes through all the foods and gives the distance for the closest one
    numFoodLeft = len(food.asList())
    numCapsulesLeft = len(currentGameState.getCapsules())
    if not normGhosts:
        closestNormGhost = 1
    else:
        closestNormGhost = min([util.manhattanDistance(pos,x.getPosition()) for x in normGhosts])
    if not scaredGhosts:
        closestScaredGhost = 1
    else:
        closestScaredGhost = min([util.manhattanDistance(pos,x.getPosition()) for x in scaredGhosts])
    #same for the ghosts

    score  = currentGameState.getScore()-2*closestFood-5*(1./closestNormGhost)-5*closestScaredGhost-25*numFoodLeft-4*numCapsulesLeft
    #the pacman should try to reduce the distance to the closest food but it should prioritize minimizing the pellets. the closestGhost is inversed because we want to increase the distance to the ghosts as much as possible. In the better function, I take into account the distance of the scared timer as well. 
    #Using graphics mode, it hangs on the last pellet, but I don't know how to fix it so I gave up

    #the actual coefficients were a result of trial and error
    return score


# Abbreviation
better = betterEvaluationFunction

