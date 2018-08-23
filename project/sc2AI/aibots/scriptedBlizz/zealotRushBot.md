---
layout: default
---
[back](./sc2APItemplate){: .btn.btn-default}

# Basic Scripted AI(Zealot Rush Bot)

[Blizzard SC2 API Tutorial Link](https://github.com/Blizzard/s2client-api/blob/master/docs/tutorial1.md){: target="_blank"}

Now, it's time to build our own bot. For this, I used Windows and Visual Studio. At the very first, we need to write scripts. In Blizzard tutiral, they added codes in "tutorial.cc". However, I recommend to add separated files for bots. For convenience, I added "jinBot.h" and "jinBot.cc" files in "sc2client-api/examples/common" since CMake add automatically files in common folder as header and source file of each example. 

	#include <sc2api/sc2_api.h>
	#include <iostream>
	using namespace sc2;

	// Protoss Scripted AI 
	class jinBot : public Agent {
	public:
	  virtual void OnGameStart() final;
	  virtual void OnStep() final;
	  bool TryBuildStructure(ABILITY_ID ability_type_for_structure, UNIT_TYPEID unit_type);
	  bool TryBuildPylon();
	  bool TryBuildGateway();
	  
	  const Unit* FindNearestMineralPatch(const Point2D& start);
	  virtual void OnUnitIdle(const Unit* unit) final;
	  virtual void OnUnitCreated(const Unit* unit) final;

	  float GetDistanceBetweenUnits(const Unit* unitFrom, const Unit* unitTo);
	  size_t CountUnitType(UNIT_TYPEID unit_type);
	};

This is jinBot.h. I select my bot race as Protoss. jinBot inherite Agent, and the virtual methods, OnGameStart ad OnStep, must be implemented. TryBuildStructure is for building a structure, and it is used by TryBuildPylon and TryBuildGateway. Further, there are more functions; FindNearMineralPatch, OnUnitIdle, OnUnitCreated, GetDistanceBetweenUnits, and CountUnitType.

	// The game start...
	void jinBot::OnGameStart(){
	  std::cout << "jinBot is running..." << std::endl;
	}

	// The tasks for each step...
	void jinBot::OnStep(){
	  TryBuildPylon();
	  TryBuildGateway();
	}

Each step, jinBot builds Pylons and Gateways based on the number of Nexus. 

	// For building structures...
	bool jinBot::TryBuildStructure(ABILITY_ID ability_type_for_structure, UNIT_TYPEID unit_type = UNIT_TYPEID::PROTOSS_PROBE){
	  const ObservationInterface* observation = Observation();
	  const Unit* unit_to_build = nullptr;
	  Units units = observation->GetUnits(Unit::Alliance::Self);
	  for (const auto& unit : units) {
	    for (const auto & order : unit->orders) {
	      if (order.ability_id == ability_type_for_structure) {
	        return false;
	      }
	    }
	    if (unit->unit_type == unit_type) {
	      unit_to_build = unit;
	    }
	  }
	  float rx = GetRandomScalar();
	  float ry = GetRandomScalar();

	  Actions()->UnitCommand(unit_to_build, ability_type_for_structure, Point2D(unit_to_build->pos.x + rx*15.0f, unit_to_build->pos.y + ry*15.0f));
	  return true;
	}

TryBuildStructure selects a unit, it must be a worker, and check the operations the unit is available to. When the selected unit is available to an building operation, UnitCommand makes the unit build a structure with a random position (x, y). TryBuildStructure uses available actions for building a specific structure. Therefore, input params must be related to the actions.

	bool jinBot::TryBuildGateway() {
	  const ObservationInterface* observation = Observation();
	  if (CountUnitType(UNIT_TYPEID::PROTOSS_PYLON) < 1 && CountUnitType(UNIT_TYPEID::PROTOSS_PROBE) < 18) return false;
	  else {
	    if ((CountUnitType(UNIT_TYPEID::PROTOSS_GATEWAY) < (5 *  CountUnitType(UNIT_TYPEID::PROTOSS_NEXUS)))) {
	      return TryBuildStructure(ABILITY_ID::BUILD_GATEWAY);
	    }
	  }
	  return false;
	}

	bool jinBot::TryBuildPylon() {
	  const ObservationInterface* observation = Observation();
	  if(observation->GetFoodCap() )
	  if (observation->GetFoodUsed() <= observation->GetFoodCap() - 5)
	    return false;
	  return TryBuildStructure(ABILITY_ID::BUILD_PYLON);
	}

TryBuildPylon and TryBuildGatway check conditions for building Pylons and Gateways, and use TryBuildStructure for building them. Since TryBuildStructure selects a worker and makes a command for building a structure, the things which TryBuildPylon and TryBuildGateway do are to check the conditions and call TryBuildStructure with proper params.

	// For resources...
	const Unit* jinBot::FindNearestMineralPatch(const Point2D& start) {
	  Units neutralUnits = Observation()->GetUnits(Unit::Alliance::Neutral);
	  Units myUnits = Observation()->GetUnits(Unit::Alliance::Self);
	  float distance = std::numeric_limits<float>::max();
	  const Unit* target = nullptr;

	  for (const auto& u : neutralUnits) {
	    if (u->unit_type == UNIT_TYPEID::NEUTRAL_MINERALFIELD) {
	      float d = DistanceSquared2D(u->pos, start);
	      if (d < distance) {
	        for (const auto & mU : myUnits) {
	          if (mU->unit_type == UNIT_TYPEID::PROTOSS_NEXUS) {
	            float distBtwMinNex = GetDistanceBetweenUnits(mU, u);
	            if (distBtwMinNex < 30) {
	              distance = d;
	              target = u;
	              return target;
	            }
	          }
	        }
	      }
	    }
	  }
	  return NULL;
	}

	// For the unit action...
	void jinBot::OnUnitIdle(const Unit* unit) {
	  switch (unit->unit_type.ToType()) {
	    case UNIT_TYPEID::PROTOSS_NEXUS: {
	      if (CountUnitType(UNIT_TYPEID::PROTOSS_PROBE) < CountUnitType(UNIT_TYPEID::PROTOSS_NEXUS)*25) {
	        Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_PROBE);
	      }
	      if (CountUnitType(UNIT_TYPEID::PROTOSS_MOTHERSHIPCORE) == 0) {
	        Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_MOTHERSHIPCORE);
	      }
	      break;
	    }
	    case UNIT_TYPEID::PROTOSS_PROBE: {
	      const Unit* mineral_target = FindNearestMineralPatch(unit->pos);
	      if (!mineral_target) {
	        break;
	      }
	      Actions()->UnitCommand(unit, ABILITY_ID::SMART, mineral_target);
	      break;
	    }
	    case UNIT_TYPEID::PROTOSS_GATEWAY: {
	      Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_ZEALOT);
	      break;
	    }
	    case UNIT_TYPEID::PROTOSS_ZEALOT: {
	      if (CountUnitType(UNIT_TYPEID::PROTOSS_ZEALOT) > 20) {
	        const GameInfo& curInfo = Observation()->GetGameInfo();
	        Actions()->UnitCommand(unit, ABILITY_ID::ATTACK_ATTACK, curInfo.enemy_start_locations.front());
	      }
	      break;
	    }
	    default: {
	      break;
	    }
	  }
	}


FindNearestMineralPatch gets the minimum distance of mineral fileds. The codes are really easy to understand. It is iterated for every mineral filed to get the distance, and gets the minimun distance. OnUnitIdle defines operations for the idle units to do something. Nexus trains probes when the number of probes is under 25. The codes for training MothershipCore is to check the version since there is no mothership after the 4.0 patch of starcraft II([link](https://news.blizzard.com/en-us/starcraft2/21183638/starcraft-ii-4-0-patch-notes)). The provided replay packs are version 3.17.X. I just want to know that I can use the replay packs for my future machine learning bot with the latest version of Starcraft II. I think the supervised bot will be unstable since the replay packs are for the past version. I ask this problem to Starcraft II progamers and Youtubers, and they answer that there are so huge differences after patch 4.0 that it is unreasonable to use 3.17.x replays for 4.x bots.

	// For 
	void jinBot::OnUnitCreated(const Unit* unit) {
	  //
	}

	float jinBot::GetDistanceBetweenUnits(const Unit* unitFrom, const Unit* unitTo) {
	  return DistanceSquared2D(unitTo->pos, unitFrom->pos);
	}

	// Count units of mine...
	size_t jinBot::CountUnitType(UNIT_TYPEID unit_type) {
	  return Observation()->GetUnits(Unit::Alliance::Self, IsUnit(unit_type)).size();
	}

These codes run two Starcraft II and make bots be against each other. Congratulation! Now, we can make more complicated bots. 
	
	jinBot bot1;
	jinBot bot2;
	coordinator.SetParticipants({
	  CreateParticipant(Race::Protoss, &bot1),
	  CreateParticipant(Race::Protoss, &bot2)
	});


The full source codes are available [here](https://github.com/jin993/SC2_bot/tree/master/blizzAPI/scripted){: target="_blank"}