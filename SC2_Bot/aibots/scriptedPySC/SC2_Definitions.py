from pysc2.lib import actions
from pysc2.lib import features

# Actions
FUNCTIONS = actions.FUNCTIONS

NO_OP = FUNCTIONS.no_op.id

SELECT_POINT = FUNCTIONS.select_point.id
SELECT_ARMY = FUNCTIONS.select_army.id

BUILD_SUPPLYDEPOT = FUNCTIONS.Build_SupplyDepot_screen.id
BUILD_BARRACKS = FUNCTIONS.Build_Barracks_screen.id

TRAIN_MARINE = FUNCTIONS.Train_Marine_quick.id

ATTACK_MINIMAP = FUNCTIONS.Attack_minimap.id

HARVEST_GATHER = FUNCTIONS.Harvest_Gather_screen.id

# Features
SCREEN_FEATURE = features.SCREEN_FEATURES

PLAYER_RELATIVE = SCREEN_FEATURE.player_relative.index
PLAYER_ID = SCREEN_FEATURE.player_id.index
UNIT_TYPE = SCREEN_FEATURE.unit_type.index

# Param Index
PLAYER_SELF = 1
PLAYER_HOSTILE = 4

TERRAN_COMMANDCENTER = 18
TERRAN_SCV = 45
TERRAN_SUPPLYDEPOT = 19
TERRAN_BARRACKS = 21
NEUTRAL_MINERAL_FIELD = 341

NOT_QUEUED = [0]
QUEUED = [1]
SELECT_ALL = [2]

SUPPLY_CAP = 4
SUPPLY_AMRY = 5

# Actions and Reward for ML
ACTION_DO_NOTHING = 'donothing'

ACTION_SELECT_SCV = 'selectscv'
ACTION_SELECT_BARRACKS = 'selectbarracks'
ACTION_SELECT_ARMY = 'selectarmy'

ACTION_BUILD_SUPPLYDEPOT = 'buildsupplydepot'
ACTION_BUILD_BARRACKS = 'buildbarracks'

ACTION_TRAIN_MARINE = 'tainmarine'

# Train SCV, WORK SCV

ACTION_ATTACK = 'attack'

T_COMMANDCENTER_PIXEL_Y = 290
T_SUPPLYDEPOT_PIXEL_Y = 70
T_SCV_PIXEL_Y = 8
T_MARINE_PIXEL_Y = 8
T_BARRACKS_PIXEL_Y = 140

smartActions = [
    ACTION_DO_NOTHING,
    ACTION_SELECT_SCV,
    ACTION_BUILD_SUPPLYDEPOT,
    ACTION_BUILD_BARRACKS,
    ACTION_SELECT_BARRACKS,
    ACTION_TRAIN_MARINE,
    ACTION_SELECT_ARMY,
    ACTION_ATTACK,
]

#for miniX in range(0, 64):
#    for miniY in range(0, 64):
#        if (miniX+1) % 16 == 0 and (miniY+1) % 16 == 0:
#            smartActions.append(ACTION_ATTACK + '_' + str(miniX-8) + '_' + str(miniY-8))


KILL_UNIT_REWARD = 0.2
KILL_BUILDING_REWARD = 0.5
BUILD_STRUCTURE_REWARD = 0
TRAIN_UNIT_REWARD = 0.0

DATA_FILE = 'sparse_agent_data'