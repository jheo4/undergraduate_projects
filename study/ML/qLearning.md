---
layout: default
---
[back](../csmain){: .btn.btn-default}

# Q-Learning

I refer these pages;
- [Q-Learning Wiki](https://en.wikipedia.org/wiki/Q-learning){: target="_blank"}
- [Markov Decision Process](https://en.wikipedia.org/wiki/Markov_decision_process){: target="_blank"}

## What Q-Learning Is

In [Reinforcement Learning](./reinforcementLearningbasic), Q-Learning is used to find the optimal action-value function as a value iteration method. Therefore, Q-Learning is a learning algorithm to find the optimal policy since the optimal action-value function is for finding the optimal policy. To understand the value iteration, the understanding of MDP(Markov Decision Process) is essential. 

### Value Iteration of Markov Decision Process

MDP is for modeling decision making mathematically. In MDP, there are some defined factors;

\\(\pi(s) : \  an \  action \  specified \  by \  a \  given \  policy \  \pi \ and \ state \ s\\)<br />
\\(P \  : \  a \  state \  transition \  function\\)<br />
\\(R \  : \  a \  reward \  function\\)

The MDP algorithms to get the optimal policy store the state with a value and a policy, \\(state(V,\ \pi) \\). This means that the value of a policy at a state. Therefore, These equations make sense to the optimal policy;

\\(\pi(s) = \max_a \\{ \sum_{s'}P_a(s, s')(R_a(s, s')+\gamma V(s')) \\} \\)<br />
\\(V(s) = \max_{s'}P_{\pi(s)}(s, s')(R_{\pi(s)}(s, s')+\gamma V(s'))\\)

The \\(\pi(s)\\) equation means that the optimal policy \\(\pi\\) can specify an action maximizing the state-transiting value with the reward and the discount factor. \\(V(s)\\) defines the value from the state s and a policy. This is the basic MDP.

The value iteration is one of MDP variants. Instead of getting the optimal policy by calculating the action value of a given policy from a state, the value iteration gets an action maximizing the value of it from a state. Consequently, it is not always to calculate \\(\pi(s)\\), but whenever it is needed.
The value iteration equation is like this;

\\(V_{i+1}(s) = max_a \\{ \sum_{s'}P_a(s, s')(R_a(s, s')+\gamma V_i(s')) \\} \\)

Instead of storing \\((state,\ (V,\ \pi)) \\), The value iteration needs only the pair \\((state, \ action)\\).

### Q-Learning and Value Iteration of MDP

Q-Learning is an algorithm for getting the optimal action-value function in the way of value iteration of MDP. Therefore, Q-learnig focuses on calculating the values of actions. 

The Q-Learning algorithm's equation is like this;

\\(Q(s_t, a_t)\ \leftarrow \ (1-\alpha)Q(s_t, a_t)\ +\ \alpha(r_t+\gamma max_a(Q(s_{t+1}, a)))\\)<br />
\\(s_t : \ the\ state\ at\ timeStep\ t\\)<br />
\\(a_t : \ the\ action\ of\ the\ state\ s_t\\)<br />
\\(r_t : \ the\ reward\ of\ the\ action\ a_t\\)<br />
\\(s_{t+1} : \ the\ state\ at\ timeStep\ t+1\ by\ the\ action\ a_t\\)<br />
\\(\alpha : \ a\ step\ size(learning\ rate)\ of\ Q-Learning\ (0<\alpha\le1)\\)<br />
\\(\gamma : \ a\ discount\ factor\ of\ Q-Learning\ (0\le\alpha\le1)\\)<br />
<br />
## Q-Learning Factors

### Learning Rate \\(\alpha\\)

The learning rate of Q-Learning is also calld as a step size since the learning rate determines the learning size. The Q-Learning equation shows that old value and the learned value are balanced by the learning rate. In other words, the learning rate \\(\alpha\\) is the weight factor of the old and the new. When the learning rate is close to 1, The old value is overrided by the learned value. In opposite, the learned value is superseded by the old value(no learning) 

### Discount Factor \\(\gamma\\)

The discount factor decides the importance of future rewards. Said otherwise, the discount factor \\(\gamma\\) is a weight factor between the current reward and the future rewards. Hence, the discount factor is close to 1, the convergence of learning algorithm takes long time(the action value diverges). The agent is myopic when the discount factor is 0(considering only the current reward).
<br />
<br />

## Implementation and Variants

As I mentioned, Q-Learning needs the tables storing the \\((state,\ action)\\) data, and is not proper to the large size of state/action space. To bury this problem, function approximation(aritificial neural network) and quantization can be available. 

The typical variants of Q-learning are deep Q-learning nd double Q-learning. 