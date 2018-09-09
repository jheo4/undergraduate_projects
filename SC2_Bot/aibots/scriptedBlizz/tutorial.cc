/************************************
*    Editor : Jin Heo
*    Date : 01-14-2018
*    Contact : 993jin@gmail.com
************************************/
#include <sc2api/sc2_api.h>
#include <iostream>
#include "jinBot.h"
using namespace sc2;
int main(int argc, char* argv[]) {
  Coordinator coordinator;
  coordinator.LoadSettings(argc, argv);

  jinBot bot1;
  jinBot bot2;
  coordinator.SetParticipants({
    CreateParticipant(Race::Protoss, &bot1),
    //CreateComputer(Race::Zerg)
    CreateParticipant(Race::Protoss, &bot2)
  });

  coordinator.LaunchStarcraft();
  coordinator.StartGame(sc2::kMapBelShirVestigeLE);

  while(coordinator.Update()){
  	
  }
  return 0;
}
