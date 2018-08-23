---
layout: default
---
[back](../../sc2ai){: .btn.btn-default}

# Scripted AI by using Blizzard API

[I refered Steven Brown's Tutorial](https://chatbotslife.com/building-a-basic-pysc2-agent-b109cde1477c){: target="_blank"}

To install PySC2, follow [this page](https://github.com/deepmind/pysc2/blob/master/README.md). I use Windows, PyCharm, and pip to set PySC2 up. After installing PySC2, it's time to begin. I don't explain the structure of PySC2 since PySC2 is wrappers of Blizzard StartCraft II API as I've mentioned [here](../../Prev/sc2). Therefore, it is necessary to understand Blizzard SC2 API to understand PySC2. In near future, I'll update the way for PySC2 to wrap Blizzard API. Let's get started to make my scripted bot!

	# Terran
	T_COMMANDCENTER = 18
	T_SCV = 45
	T_MARINE = 48
	T_BARRACKS = 21

	T_COMMANDCENTER_PIXEL_Y = 290
	T_SCV_PIXEL_Y = 8
	T_MARINE_PIXEL_Y = 8
	T_BARRACKS_PIXEL_Y = 140

These codes are about the unit id of Blizzard API, and I get the pixel information of units to number them. This information is used in other functions.

	PLAYER_RELATIVE = features.SCREEN_FEATURES.player_relative.index
	UNIT_TYPE = features.SCREEN_FEATURES.unit_type.index

	NUM_OF_IDLE_WORKERS = 7
	NUM_OF_ARMIES = 8
	NUM_OF_WORKERS = 6
	SUPPLY_USED = 3
	SUPPLY_MAX = 4

These variables are the feature indexs of PySC2. To get the feature information, these indexs are needed.

	PLAYER_SELF = 1

	NOT_QUEUED = [0]
	QUEUED = [1]

	# Normal Operation ID
	NO_OPERATION = actions.FUNCTIONS.no_op.id
	SELECT_POINT = actions.FUNCTIONS.select_point.id
	SELECT_ARMY = actions.FUNCTIONS.select_army.id

	SELECT_IDLE_WORKER = actions.FUNCTIONS.select_idle_worker.id


	ATTACK_MINIMAP = actions.FUNCTIONS.Attack_minimap.id
	RALLY_UNITS_MINIMAP = actions.FUNCTIONS.Rally_Units_minimap.id


	# Terran Building Object ID...
	BUILD_BARRACKS = actions.FUNCTIONS.Build_Barracks_screen.id
	BUILD_SUPPLY_DEPOT = actions.FUNCTIONS.Build_SupplyDepot_screen.id
	HARVEST_GATHER_SCREEN = actions.FUNCTIONS.Harvest_Gather_screen.id

	# Terran Training Unit ID...
	TRAIN_MARINE = actions.FUNCTIONS.Train_Marine_quick.id
	TRAIN_SCV = actions.FUNCTIONS.Train_SCV_quick.id

These are the function IDs of PySC2. The actions of Blizzard API are wrapped, and the action IDs are required to do the actions. PLAYER_SELF and QUEUED options are the parameters of action functions.

	class SimpleAgent(base_agent.BaseAgent):
    isBaseTopLeft = None
    # Relative Location From My Base
    def getRelativeLocation(self, _x, _xDist, _y, _yDist):
      """getRelativeLocation(_x : objectX, _xDist : distanceX from objectX, _y : objectY, _yDist : distanceY from obejctY) : get the relative distance by using base location
      Prerequisite : Users' base location"""
      if self.isBaseTopLeft:
          return [_x+_xDist, _y+_yDist]
      return [_x-_xDist, _y-_yDist]

With the above variables and BaseAgent provided PySC2, The first function is to get the relative XY distances from my base. SimpleAgent is implemented for SimpleMap 64x64 and, in the SimpleMap 64x64, the bases are located in eleven and five. Therefore, I get the relative location by addition when my base is TopLeft(eleven). In contrast, I calculate the relative ocation by subtraction when my location is at five.

	# Select Unit
	  def selectUnit(self, _obs, _selectUnitID):
	    """selectUnit(_obs : observation of step, _selectUnitID : unit ID of SC2 API)
	    Prerequisite : None"""
	    _unitType = _obs.observation["screen"][UNIT_TYPE]
	    _unitY, _unitX = (_unitType == _selectUnitID).nonzero()

	    _units = []
	    _numOfUnits = self.countUnits(_obs, _selectUnitID)

	    for i in range(0, len(_unitY)-1):
	      _units.append((_unitX[i], _unitY[i]))

	    _kMeans = KMeans(n_clusters = _numOfUnits)
	    _kMeans.fit(_units)

	    _i = randint(0, _numOfUnits-1)

	    _xPoint = int(_kMeans.cluster_centers_[_i][0])
	    _yPoint = int(_kMeans.cluster_centers_[_i][1])

	    _target = [_xPoint, _yPoint]
	    return actions.FunctionCall(SELECT_POINT, [NOT_QUEUED, _target])

the selectUnit function uses [K-Means clustering]("https://en.wikipedia.org/wiki/K-means_clustering"). At the very first, this function gets the total X, Y postions of a type of units. These positions belong to the all units of that type and, K-Means clustering is used to cluster the pixels of each unit. With the clustered result, it is possible to select a specific unit or random units. 

	# Select Army
	  def selectArmy(self, _obs):
	    """selectArmy(_obs : observation of step) : select all armies
	    Prerequisite : None"""
	    if SELECT_ARMY in _obs.observation['available_actions']:
	      return actions.FunctionCall(SELECT_ARMY, [NOT_QUEUED])

	  oppositeBase = []
	  # Go Rush
	  def goRush(self, _obs):
	    """goRush(_obs : observation of step) : go Rush to the opposite base
	    Prerequisite : select army"""
	    if ATTACK_MINIMAP in _obs.observation['available_actions']:
	      return actions.FunctionCall(ATTACK_MINIMAP, [NOT_QUEUED, self.oppositeBase])

More than the selectUnit function, a function to select armies is necessary to control armies. Since Blizzard API provides an action to select whole armies, I can use it for my basic bot. The reason why I select armies is to attack my enemy. Therefore, I also implement goRush function by using the oppsiteBase variable. 

	# Build Object
	  def buildObject(self, _obs, _buildActionID):
	    """buildObject(_obs : observation of step, _buildActionID : build function ID of action.py) : build object following the action ID
	    Prerequisite : select a worker"""
	    if len(_obs.observation['single_select']) > 0 and _obs.observation['single_select'][0][0] == T_SCV:
	      if _buildActionID in _obs.observation["available_actions"]:
	        _unitType = _obs.observation["screen"][UNIT_TYPE]
	        _unitY, _unitX = (_unitType == T_COMMANDCENTER).nonzero()
	        _target = self.getRelativeLocation(int(_unitX.mean()), randint(-20, 20), int(_unitY.mean()), randint(-20, 20))
	        return actions.FunctionCall(_buildActionID, [NOT_QUEUED, _target])

	    return actions.FunctionCall(NO_OPERATION, [])

the buildObject function is used to build an object with a selected worker. I select my race as Terran, so check that the selected unit is T_SCV or not. If so, build the given object in a random location near T_COMMANDCENTER.

	# Mine Mineral
	  def mineMineral(self, _obs):
	    """mineMineral(_obs : observation of step) : find the mineral field, and gather it
	     Prerequisite : select workers such as SCV, Drone, and Probe"""
	    if len(_obs.observation['single_select']) > 0 and _obs.observation['single_select'][0][0] == T_SCV:
	      _unitType = _obs.observation["screen"][UNIT_TYPE]
	      _unitY, _unitX = (_unitType==N_MINERALFIELD).nonzero()
	      _i = randint(0, len(_unitY)-1)
	      _target = [_unitX[_i], _unitY[_i]]
	      return actions.FunctionCall(HARVEST_GATHER_SCREEN, [NOT_QUEUED, _target])
	    return actions.FunctionCall(NO_OPERATION, [])

	  # Count Unit
	  def countUnits(self, _obs, _unitID):
	    """countUnits(_obs : observation of step, _unitID : count unit ID) : count units in screen"""
	    # What I need is Size of CommandCenter, SCV, Marines and Barracks
	    _unitType = _obs.observation["screen"][UNIT_TYPE]
	    _unitY, _unitX = (_unitType == _unitID).nonzero()
	    if _unitID == T_SCV :
	      return int( ceil( len(_unitY)/T_SCV_PIXEL_Y ) )
	    elif _unitID == T_COMMANDCENTER :
	      return int( ceil( len(_unitY)/T_COMMANDCENTER_PIXEL_Y ) )
	    elif _unitID == T_BARRACKS :
	      return int( ceil( len(_unitY)/T_BARRACKS_PIXEL_Y ) )
	    elif _unitID == T_MARINE :
	      return int( ceil( len(_unitY)/T_MARINE_PIXEL_Y ) )
	    else :
	      return -1

	  # Train Unit
	  def trainUnit(self, _obs, _trainUnit):
	  	"""trainUnit(_obs : observation of step, _trainUnit : unit ID for training) : count units in screen
	  	 Prerequisite : select the barracks or any kind of the training building.
	  	"""
	    if (_obs.observation["player"][SUPPLY_USED] < _obs.observation["player"][SUPPLY_MAX]) and _trainUnit in _obs.observation["available_actions"]:
	      return actions.FunctionCall(_trainUnit, [NOT_QUEUED])
	    return actions.FunctionCall(NO_OPERATION, [])

These functions are used to mine mineral, count units, and train units. These codes are easy to understand. The method to count units uses the pixel information. Since observation gives the whole pixels of any type of units, I can get the number of units by pixel division. 

	  isIdleWorkerSelected = False
	  isSupplyDepotNeeded = False
	  isArmySelected = False
	  isBarracksSelected = False
	  def step(self, obs):
	    super(SimpleAgent, self).step(obs)
	    '''Basic Operations...'''
	    # Get my base location and opposite base location...
	    if self.isBaseTopLeft is None:
	      baseY, baseX = (obs.observation["minimap"][PLAYER_RELATIVE] == PLAYER_SELF).nonzero()
	      self.isBaseTopLeft = baseY.mean() <= 31
	      self.oppositeBase = [64-int(baseX.mean()), 64-int(baseY.mean())]

	    # Check supply depots are needed...
	    if obs.observation["player"][SUPPLY_USED] + 4 > obs.observation["player"][SUPPLY_MAX]:
	      self.isSupplyDepotNeeded = True

	    # Make idle workers gather mineral...
	    if obs.observation["player"][NUM_OF_IDLE_WORKERS] > 0:
	      if not self.isIdleWorkerSelected:
	        self.isIdleWorkerSelected = True
	        return actions.FunctionCall(SELECT_IDLE_WORKER, [NOT_QUEUED])
	      else:
	        self.isIdleWorkerSelected = False
	        return self.mineMineral(obs)

	    '''Operations...'''
	    # Build supply depots...
	    if self.isSupplyDepotNeeded:
	      if obs.observation['single_select'][0][0] != T_SCV:
	        return self.selectUnit(obs, T_SCV)
	      else:
	        self.isSupplyDepotNeeded = False
	        return self.buildObject(obs, BUILD_SUPPLY_DEPOT)

	    # Train SCV with Num Of Command Center
	    elif obs.observation['player'][NUM_OF_WORKERS] < 20 and (obs.observation["player"][SUPPLY_USED] < obs.observation["player"][SUPPLY_MAX]):
	      if obs.observation['single_select'][0][0] != T_COMMANDCENTER:
	        return self.selectUnit(obs, T_COMMANDCENTER)
	      else :
	        return self.trainUnit(obs, TRAIN_SCV)

	    # Build CommandCenter*4 barracks...
	    elif self.countUnits(obs, T_BARRACKS) < self.countUnits(obs, T_COMMANDCENTER) * 4:
	      if obs.observation['single_select'][0][0] != T_SCV:
	        return self.selectUnit(obs, T_SCV)
	      else:
	        return self.buildObject(obs, BUILD_BARRACKS)

	    # Train marines
	    elif obs.observation['player'][NUM_OF_ARMIES] < 15 :
	      if obs.observation['single_select'][0][0] != T_BARRACKS and not self.isBarracksSelected:
	        self.isBarracksSelected = True
	        return self.selectUnit(obs, T_BARRACKS)
	      else :
	        self.isBarracksSelected = False
	        return self.trainUnit(obs, TRAIN_MARINE)

	    # Go rush
	    elif obs.observation['player'][NUM_OF_ARMIES] >= 15 :
	      if not self.isArmySelected :
	        self.isArmySelected = True
	        return self.selectArmy(obs)
	      else :
	        if obs.observation['single_select'][0][0] != T_MARINE and self.isArmySelected:
	          self.isArmySelected = False
	          return self.goRush(obs)

	    return actions.FunctionCall(NO_OPERATION, [])

the step function is executed at every time step of a game. In this function, I check my base location by using minimap, build supplydepots, and make idle workers work. I classify these operations as basic operations since they must be executied without regard to any situation. 
With the basic operations, there are bunch of other operations. They are executed with some conditions, and also have some priorities. For instance, goRush function must be executed next to trainMarine function. These piorities are commented as "Prerequisite of each functions".

This is the basic marine rush bot with PySC2. The reason why I implement this scripted bot is to get used to using PySC2 in my future AIs.

The full source codes are available [here](https://github.com/jin993/SC2_bot/tree/master/PySC2/scripted){: target="_blank"}