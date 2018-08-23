---
layout: default
---
[back](./structureSC2API){: .btn.btn-default} 
[Basic Scripted AI(Zealot Rush Bot)](./zealotRushBot){: .btn.btn-default}

# Get Started with SC2 API Template

[Blizzard SC2 API Tutorial Link](https://github.com/Blizzard/s2client-api/blob/master/docs/tutorial1.md){: target="_blank"}

To start with the basic template of a SC2 bot, I explain the tutorial.cc file of SC2 API.

In main

	Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

Coordinator coordinate clients. LoadSetting function patch the input options of a client, and configure the client with that options.  

	Bot bot;
	coordinator.SetParticipants({
	    CreateParticipant(Race::Terran, &bot),
	    CreateComputer(Race::Zerg)
	});

This codes create a bot object, and assign it as on the player side. The opposite player is a zerg computer. In this case, Bot class has scripted operations for playing StarCraft II. When we write some codes on it, we can build our own bot.

	coordinator.LaunchStarcraft();
    coordinator.StartGame(sc2::kMapBelShirVestigeLE);

    while(coordinator.Update()){
    }

After setting participants, coordinator launches StarCraft II, and starts a game wit the map parameter. The whole maps are available at "s2client-api/include/sc2api/sc2_game_settings.h" and "s2client-api/src/sc2api/sc2_game_settings.cc". These files are built by CMake. Therefore, it is required to rebuild sc2client-api when we add new maps. 
When the game start, the coordinator calls the Update function for stepping the game.

![SC2 Game State](../../Plat/BlizAPI/gameDiagram.png)

Each step follows this diagram sequence. Observation and Action are the core of steps. 

	class Bot : public Agent {
	public:
	    virtual void OnGameStart() final {
	        std::cout << "Hello, World!" << std::endl;
	    }

	    virtual void OnStep() final {
	        std::cout << Observation()->GetGameLoop() << std::endl;
	    }
	};

The codes of toturial bot are extreamly simple. The OnGameStart function is used to execute operations at the very first time of a game. Moreover, the OnStep function is used to specify the actions based on observation information. 

These codes are the very basic form of a SC2 API bot. It's an entry point to start SC2 API, Hello SC2 API!