/************************************
*    Editor : Jin Heo
*    Date : 01-14-2018
*    Contact : 993jin@gmail.com
************************************/

#include <sc2api/sc2_api.h>
#include <iostream>
using namespace sc2;

// Protoss Scripted AI 
class junBot : public Agent {
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
