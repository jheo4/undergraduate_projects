---
layout: default
---
[back](../sc2ai){: .btn.btn-default}

# A Survey of Real-Time Strategy Game AI Research and Competition in StarCraft

[Paper link](http://ieeexplore.ieee.org/document/6637024/){: target="_blank"}

## This Paper Is
This paper is related to the Starcraft 1 AI research. The reason why I read this paper is that I need to know the current research topic of Starcraft AI, and find the field which I can contribute to.

## Motivation
At the very first step of this research, I have to figure out characteristics of Starcraft. Starcraft is a RTS(Real Time Strategy) game, so a player is required to make decisions every moment with the information the player has. Unlike board games which the players can get the whole information of the game in, it is impossible to get the whole information because of Fog-of-War in Starcraft. So, it makes the AI player hard to be like a human player. Moreover, the complexity of Starcraft is much bigger than other board games. 

## The Challenges in RTS AI and The Way to Get Through The Challenges
Following this paper there are six challenges in RTS AI.
1. Resource management
1. Decision making under uncertainty
1. Spatial and temporal reasoning
1. Collaboration (between multiple AIs)
1. Opponent modeling and learning
1. Adversarial real-time planning
Decision making is a really important challenge. To tackle this, task decomposition is preferable. I'll focus on the decision making of AI players. Tasks are decomposed into 5 categories.
- Strategy
- Tactics
- Reactive control 
- Terrain analysis
- Intelligence gathering
In this paper, the researches of each category are introduced.
- Strategy 
	- Hard cod approach
		- FSM(Finite State Machine)
		- CBP(Case Based Planning)
	- Machine learning approach
		- Supervised learning
		- Semi-supervised learning
		- Unsupervised learning
		- CBR(Case Based Reasoning)
- Tactics : by using map information. 
- Reactive Control
	- Path finding
	- Avoid obstacles, opponent fire, and stay at maximum shooting distance

## For The Future
I think that decision making is most important for making AI players, and strategy is the most important for decision making. I played Starcraft 1 during my high school days to be a programmer, so I well know Starcraft 1. After mastering the basic of Starcraft such as spending resources, making supply buildings, and training the army continuously, the player who want to win the game should make right decisions at the right time. So, I'll focus on theses to make AI player like a human, eventhough the AI player's tactics or reactive control is not great.