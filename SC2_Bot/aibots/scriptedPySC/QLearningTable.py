import pandas as pd
import numpy as np

class QLearningTable:
    # init function for QLearningTable

    # Q Table for ML
    #         action1 action2 ...
    # state1   float   float  ...
    # state2   float   float
    #  ...      ...     ...

    def __init__(self, actions, learningRate=0.02, rewardDecay=0.9, eGreedy=0.9):
        self.actions = actions
        self.lr = learningRate
        self.gamma = rewardDecay
        self.epsilon = eGreedy
        self.Qtable = pd.DataFrame(columns=self.actions, dtype=np.float64)


    # checkStateExist
    def checkStateExist(self, state):
        if state not in self.Qtable.index:
            # append new state to q table
            self.Qtable = self.Qtable.append(pd.Series([0] * len(self.actions), index=self.Qtable.columns, name=state))

    # chooseAction function
    def chooseAction(self, observation):
        self.checkStateExist(observation)
        # select the best action
        if np.random.uniform() < self.epsilon:
            stateAction = self.Qtable.ix[observation, :]
            stateAction = stateAction.reindex(np.random.permutation(stateAction.index))
            action = stateAction.idxmax()

        # select a random action
        else:
            action = np.random.choice(self.actions)

        return action

    def checkStateExist(self, state):
        if state not in self.Qtable.index:
            self.Qtable = self.Qtable.append(pd.Series([0]*len(self.actions), index=self.Qtable.columns, name=state))

    # learn function
    def learn(self, s, a, r, s_):
        # s : state at step t
        # a : available actions of state s at step t
        # r : reward from the action
        # s_ : state by an action at step t
        self.checkStateExist(s_)
        self.checkStateExist(s)

        # get the predicted value of an action
        Qpredict = self.Qtable.ix[s, a]

        # get the target value V = r + V(s_)
        Qtarget = r + (self.gamma * self.Qtable.ix[s_, :].max())

        #if s_ != 'terminal':
        #    Qtarget = r + (self.gamma * self.Qpredict.ix[s_, :].max())
        #else:
        #    Qtarget = r

        self.Qtable.ix[s, a] += (self.lr * (Qtarget - Qpredict))






