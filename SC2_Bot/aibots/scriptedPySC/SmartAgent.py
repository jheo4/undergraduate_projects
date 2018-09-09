import random
import math
from os import *
import numpy as np
import pandas as pd

from pysc2.agents import base_agent
from pysc2.lib import actions
from pysc2.lib import features

from SC2_Definitions import *
from QLearningTable import *
from SC2_Functions import *


class SmartAgent(base_agent.BaseAgent):
    supplyX = 0
    supplyY = 13
    barrackX = 13
    barrackY = 0
    def __init__(self):
        super(SmartAgent, self).__init__()
        self.Qlearn = QLearningTable(actions=list(range(len(smartActions))))

        self.previous_killedUnitScore = 0
        self.previous_killedBuildingScore = 0
        self.previous_structureScore = 0
        self.previous_unitScore = 0

        self.previous_action = None
        self.previous_state = None

        self.moveNumber = 0

        if path.isfile(DATA_FILE + '.gz'):
            self.Qlearn.Qtable = pd.read_pickle(DATA_FILE + 'gz', compression='gzip')

    def getRelativeDistance(self, x, xDist, y, yDist):
        if self.baseTopLeft:
            return [x+xDist, y+yDist]
        return [x-xDist, y-yDist]

    def getRelativeLocation(self, x, y):
        if self.baseTopLeft:
            return [x, y]
        return [64-x, 64-y]


    def step(self, obs):
        super(SmartAgent, self).step(obs)

        # set the base location
        pY, pX = (obs.observation['minimap'][PLAYER_RELATIVE] == PLAYER_SELF).nonzero()
        self.baseTopLeft = 1 if pY.any() and pY.mean() <= 31 else 0

        # States...
        unitType = obs.observation['screen'][UNIT_TYPE]
        depotY, depotX = (unitType == TERRAN_SUPPLYDEPOT).nonzero()
        self.numOfSupplyDepot = int(math.ceil(len(depotY)/T_SUPPLYDEPOT_PIXEL_Y))

        barracksY, barraksX = (unitType == TERRAN_BARRACKS).nonzero()
        self.numOfBarracks = int(math.ceil(len(barracksY)/T_BARRACKS_PIXEL_Y))

        supplyLimit = obs.observation['player'][4]
        armySupply = obs.observation['player'][5]

        unitScore = obs.observation['score_cumulative'][3]
        structureScore = obs.observation['score_cumulative'][4]
        killedUnitScore = obs.observation['score_cumulative'][5]
        killedBuildingScore = obs.observation['score_cumulative'][6]

        currentState = np.zeros(4)
        currentState[0] = self.numOfSupplyDepot
        currentState[1] = self.numOfBarracks
        currentState[2] = supplyLimit
        currentState[3] = armySupply

        #hostileRegion = np.zeros(16)
        #enemyY, enemyX = (obs.observation['minimap'][PLAYER_RELATIVE] == PLAYER_HOSTILE).nonzero()

        #for i in range(0, len(enemyY)-1):
        #    y = int(math.ceil((enemyY[i] % 64) / 16))
        #    x = int(math.ceil((enemyX[i] % 64) / 16))

        #    hostileRegion[((y - 1) * 4) + (x - 1)] = 1

        #if not self.baseTopLeft:
        #    hostileRegion = hostileRegion[::-1]

        #for i in range(0, 16):
        #    currentState[i+4] = hostileRegion[i]

        if self.previous_action is not None:
            reward = 0
            if killedUnitScore > self.previous_killedUnitScore:
                reward += KILL_UNIT_REWARD
            if killedBuildingScore > self.previous_killedBuildingScore:
                reward += KILL_BUILDING_REWARD
            if unitScore > self.previous_unitScore:
                reward += TRAIN_UNIT_REWARD
            if structureScore > self.previous_structureScore:
                #reward += BUILD_STRUCTURE_REWARD
                pass
            self.Qlearn.learn(str(self.previous_state), self.previous_action, reward, str(currentState))

        QlearnedAction = self.Qlearn.chooseAction(str(currentState))
        smartAction = smartActions[QlearnedAction]

        self.previous_structureScore = structureScore
        self.previous_unitScore = unitScore
        self.previous_killedBuildingScore = killedBuildingScore
        self.previous_killedUnitScore = killedUnitScore
        self.previous_action = QlearnedAction
        self.previous_state = currentState

        #miniX = 0
        #miniY = 0
        #if '_' in smartAction:
        #    smartAction, miniX, miniY = smartAction.split('_')

        if smartAction == ACTION_DO_NOTHING:
            return actions.FunctionCall(NO_OP, [])

        elif smartAction == ACTION_SELECT_SCV:
            unitType = obs.observation['screen'][UNIT_TYPE]
            unitY, unitX = (unitType == TERRAN_SCV).nonzero()
            if unitY.any():
                i = random.randint(0, len(unitY)-1)
                target = [unitX[i], unitY[i]]

                return actions.FunctionCall(SELECT_POINT, [NOT_QUEUED, target])

        elif smartAction == ACTION_BUILD_SUPPLYDEPOT:
            if BUILD_SUPPLYDEPOT in obs.observation['available_actions']:
                unitType = obs.observation['screen'][UNIT_TYPE]
                unitY, unitX = (unitType == TERRAN_COMMANDCENTER).nonzero()
                if unitY.any():
                    target = self.getRelativeDistance(int(unitX.mean()), self.supplyX, int(unitY.mean()), self.supplyY)
                    if self.numOfSupplyDepot == 0:
                        self.supplyY = 13
                        self.supplyX = 0
                    elif self.numOfSupplyDepot == 1:
                        self.supplyY = 20
                        self.supplyX = 0
                    if self.numOfSupplyDepot == 2:
                        self.supplyY = 13
                        self.supplyX = 13
                    elif self.numOfSupplyDepot == 3:
                        self.supplyY = 20
                        self.supplyX = 13
                    return actions.FunctionCall(BUILD_SUPPLYDEPOT, [NOT_QUEUED, target])

        elif smartAction == ACTION_BUILD_BARRACKS:
            if BUILD_BARRACKS in obs.observation['available_actions']:
                unitType = obs.observation['screen'][UNIT_TYPE]
                unitY, unitX = (unitType == TERRAN_COMMANDCENTER).nonzero()
                if unitY.any():
                    target = self.getRelativeDistance(int(unitX.mean()), self.barrackX, int(unitY.mean()), self.barrackY)
                    if self.numOfBarracks == 0:
                        self.barrackX = 13
                        self.barrackY = 0
                    elif self.numOfBarracks == 1:
                        self.barrackX = 24
                        self.barrackY = 0
                    return actions.FunctionCall(BUILD_BARRACKS, [NOT_QUEUED, target])

        elif smartAction == ACTION_SELECT_BARRACKS:
            unitType = obs.observation['screen'][UNIT_TYPE]
            unitY, unitX = (unitType == TERRAN_BARRACKS).nonzero()
            if unitY.any():
                i = random.randint(0, len(unitY)-1)
                target = [unitX[i], unitY[i]]

                return actions.FunctionCall(SELECT_POINT, [NOT_QUEUED, target])

        elif smartAction == ACTION_TRAIN_MARINE:
            if TRAIN_MARINE in obs.observation['available_actions']:
                return actions.FunctionCall(TRAIN_MARINE, [QUEUED])

        elif smartAction == ACTION_SELECT_ARMY:
            if SELECT_ARMY in obs.observation['available_actions']:
                return actions.FunctionCall(SELECT_ARMY, [NOT_QUEUED])

        elif smartAction == ACTION_ATTACK:
            if ATTACK_MINIMAP in obs.observation['available_actions'] and obs.observation['single_select'][0][0] != TERRAN_SCV:
                if self.baseTopLeft:
                    return actions.FunctionCall(ATTACK_MINIMAP, [NOT_QUEUED, [39, 45]])
                return actions.FunctionCall(ATTACK_MINIMAP, [NOT_QUEUED, [21, 24]])

        return actions.FunctionCall(NO_OP, [])
