/************************************
*    Editor : Jin Heo
*    Date : 01-14-2018
*    Contact : 993jin@gmail.com
************************************/

#include "junBot.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <iterator>

#include "sc2api/sc2_api.h"
#include "sc2lib/sc2_lib.h"

// The game start...
void junBot::OnGameStart(){
  std::cout << "junBot is running..." << std::endl;
}

// The tasks for each step...
void junBot::OnStep(){
  TryBuildPylon();
  TryBuildGateway();
}

// For building structures...
bool junBot::TryBuildStructure(ABILITY_ID ability_type_for_structure, UNIT_TYPEID unit_type = UNIT_TYPEID::PROTOSS_PROBE){
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

bool junBot::TryBuildGateway() {
  const ObservationInterface* observation = Observation();
  if (CountUnitType(UNIT_TYPEID::PROTOSS_PYLON) < 1 && CountUnitType(UNIT_TYPEID::PROTOSS_PROBE) < 18) return false;
  else {
    if ((CountUnitType(UNIT_TYPEID::PROTOSS_GATEWAY) < (5 *  CountUnitType(UNIT_TYPEID::PROTOSS_NEXUS)))) {
      return TryBuildStructure(ABILITY_ID::BUILD_GATEWAY);
    }
  }
  return false;
}

bool junBot::TryBuildPylon() {
  const ObservationInterface* observation = Observation();
  if(observation->GetFoodCap() )
  if (observation->GetFoodUsed() <= observation->GetFoodCap() - 5)
    return false;
  return TryBuildStructure(ABILITY_ID::BUILD_PYLON);
}

// For resources...
const Unit* junBot::FindNearestMineralPatch(const Point2D& start) {
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
void junBot::OnUnitIdle(const Unit* unit) {
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

// For 
void junBot::OnUnitCreated(const Unit* unit) {
  //
}

float junBot::GetDistanceBetweenUnits(const Unit* unitFrom, const Unit* unitTo) {
  return DistanceSquared2D(unitTo->pos, unitFrom->pos);
}

// Count units of mine...
size_t junBot::CountUnitType(UNIT_TYPEID unit_type) {
  return Observation()->GetUnits(Unit::Alliance::Self, IsUnit(unit_type)).size();
}
