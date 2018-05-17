# Huidong Hwang and Chris Knoll
# 03/21/16
# Takes in a sequence of states and their actions and returns the J values and their optimal policies

import sys
from collections import OrderedDict #To have dictionaries in order
from decimal import Decimal #To prevent rounding errors with .5

def parse(filename):
    states = OrderedDict() #Initializes the dictionary
    with open(filename,"r+") as f:
        for line in f:
            line = line.strip() #Strips the extra whitespace
            if line == "": #Strips empty lines
                continue
            state,value,parens = line.split(' ', 2) #Splits the line to three values, parens to be parsed later
            transitions = [f.split(" ") for f in parens[1:-1].split(') (')] #1:-1 removes the first and last parentheses
                                                                            #Parses transitions into lists
            actions = OrderedDict()
            for trans in transitions:
                if trans[0] not in actions:
                    actions[trans[0]] = []
                actions[trans[0]].append((trans[1], Decimal(trans[2]))) #Parses to a dict with actions and 2-tuples with state_j and the probabilities
            states[state] = (int(value),actions) #Inserts into a dict with state, and a 2-tuple with reward value and the dict
    return states

def jval(n, states, gamma):
    j = {}

    for s in states: #Initializes the list with reward values
        j[s] = states[s][0]

    for i in range(1,21):
        print("After iteration {}:".format(i))
        jk = j.copy()
        for si in states:
            val, optact =  max([(sum([sj[1] * jk[sj[0]] #Max of the sum of the probabilities times the J values of state_j
                                for sj in states[si][1][act]]),act) #Parsed into a 2-tuple of the max and optimal action
                                    for act in states[si][1]]) #For all the actions for a state
            if i != 1:
                j[si] = states[si][0] + gamma * val #Bellman's Equation
            print("({} {} {:.4f})".format(si,optact,round(j[si],4)),end=" ") #Output formatting
        print()
    pass


def main():
    if len(sys.argv) - 1 != 4:
        print("Arguments need to be exactly 4")
        sys.exit()
    states = parse(sys.argv[3]) #Parses the datastream
    jval(20, states, Decimal(sys.argv[4])) #Computes the J values and prints the output
    return

if __name__ == "__main__":
    main()
