---
layout: default
---
[back](../sc2ai){: .btn.btn-default}

# Benchmarking Deep Reinforcement Learning for Continuous Control

[Paper link](https://arxiv.org/pdf/1604.06778){: target="_blank"}

## This Paper Is
This paper is about benchmarking of deep reinforcement learning algorithms. This paper has meaning in my project. First of all, I should know how to benchmark the learning algorithm. In this paper, the researchers introduce exisiting algorithms and how to evaluate them. Since I just start the first step of this project, I shold know previous successful researches and the characteristics of them for my interest.

## Motivation
The meaning of reinforcement learning is that "address how agents should learn to take actions to maximize cumulative reward through interactions with the environment". These days, deep learning and reinforcement learning are combined, and show great results. Further, the continuous control makes complexity high. In my project, the target game is SC2(StarCraft 2). If I can get some commonalities between my situation and the situation this paper assume, it would be helpful. 

## Basic of The Reinforcement Learning
For reinforcement learning, the policy is really important. The equation of the policy is defined by MDP(Markov Deicision Process):

\\(\pi : policy \qquad\\)
\\(S : states \qquad\\)
\\(A : actions \qquad\\)
\\(\rho : pdf \qquad\\)
\\(r : reward fuctions \qquad\\)
\\(R : reward values \qquad\\)

\\( P = S \times A \times S \to R : the\ transition\ probability\ distribution\\)

this means the transition from one state to the other state by action. Therefore, this is transition probability by an action.

\\(r = S \times A \to R \\)

\\(\pi = S \times A \to R_{\geq0} \\)

After defining the policy, the continuous feature should be considered. 

\\(\gamma : the\ discount\ factor \\)

\\(\eta(\pi) = E_r[\sum_{t=0}^T\gamma^tr(s_t, a_t)]\\)

## The Tasks Applying Reinforcement Learning To
In this paper, the tasks are categorized into:

1. Basic tasks : low dimensional tasks 
1. Locomotion tasks : the moving forward tasks as quickly as possible
1. Partially observable tasks : the states are partially observable.
	- Limited sensors
	- Noisy observations and delayed actions 
	- System identification
1. Hierarchical tasks : higher level dicisions can reuse lower level skills.

## For The Future
I think partially observable tasks have the similarity with my project. It can be possible to see SC2 as the situation with limited sensors, noisy obervation, and delayed actions. the SC2 player get imperfect information because of Fog-of-War, and this situation is very similar to the limited sensor situation. Further, strategis are usually in effect after a couple of minutes from the previous decision. Past observations and past actions should be used to infer the current state, and current states and current actions should be used to infer the future state. The researchers apply various algorithm to these tasks. There are many algorithms, and each algorithm has its own way to get the policy. 

I think the algorithms for partially observable tasks should be considered for this project. It seems to have similarities with SC2. Therefore, something helpful is in that research.