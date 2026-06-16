---
title: Strengthening Reinforcement Learning Agents in with Multi-Episode Evaluation and Validation Phases
permalink: /tutos/strengthening-agents
toc: true
toc_sticky: true
---

The objective of this tutorial is two-fold:
1. Strengthen the built reinforcement learning agents by evaluating them over multiple episodes during training, and
2. Activate a validation phase at the end of each generation to monitor potential overfitting, and

The starting point of this tutorial is the C++ project obtained at the end of the _[GEGELATI introductory tutorial](/gegelati-tutorial)_. While completing the introductory tutorial is strongly advised, a copy of the project resulting from this tutorial can be downloaded at the following link: [pendulum_wrapper_solution.zip](/gegelati-tutorial/data/gegelati-tutorial-solution.zip).

## 0. Multi-episode evaluation of agents

### Why evaluate over multiple episodes?
An episode refers to a complete sequence of interactions between a reinforcement learning agent and its environment, starting from an initial state and ending when a terminal condition is met. For example, in the initial tutorial, an episode consists of the agent attempting to balance the pendulum for a fixed duration of 1500 time steps, as defined by the `maxNbActionsPerEval` parameter in `params.json`.

In reinforcement learning, evaluating an agent's performance over multiple episodes is crucial for obtaining a reliable evaluation of its true capabilities. This is because the performance of an agent can vary significantly from one episode to another due to the inherent stochasticity of the environment and the agent's policy. By averaging the results over multiple episodes, we can mitigate the effects of randomness and strengthen the robustness of the learned policy.

Implementing multi-episode evaluation in <span style="font-variant: small-caps;">Gegelati</span> involves modifying the `PendulumWrapper` class to support multiple episodes during the evaluation phase. To vary the starting conditions of each episode, the pendulum's angle and angular velocity will be randomly initialized at the beginning of each episode.

### Modify PendulumWrapper to support multi-episode evaluation
To implement multi-episode evaluation, we will first modify the `PendulumWrapper` class to support a stochastic reset of the pendulum's state at the beginning of each episode.

To support random initialization, we will use a pseudo-random number generator to generate random values for the pendulum's angle and angular velocity within specified ranges.

#### TODO #1:
Edit the `/gegelati-tutorial/src/environments/pendulum_wrapper.h`to add a random number generator as a member variable of the `PendulumWrapper` class. This pseudo-random number generator is provided in <span style="font-variant: small-caps;">Gegelati</span> with the `Mutator::RNG` class.

{% details Solution to #1 (Click to expand) %}
```cpp
/* pendulum_wrapper.h */
class PendulumWrapper : public Learn::LearningEnvironment {
public:
    // Existing code...

    /// Random Number Generator for the environment
	Mutator::RNG rng;
```

{% enddetails %}

#### TODO #2:
Next, we will modify the `reset(size_t seed, Learn::LearningMode mode, uint16_t iterationNumber, uint64_t generationNumber)` method of the `PendulumWrapper` class to randomly initialize the pendulum's angle and angular velocity at the beginning of each episode.

When calling the `reset(...)` method of the environment, <span style="font-variant: small-caps;">Gegelati</span> notably provides a `seed` parameter that can be used to seed the environment random number generator, using the `Mutator::RNG::setSeed(size_t seed)` method. The provided `seed` value is unique for each iteration and generation. Using this seeding mechanism ensures deterministic reproducibility of the random initialization across different runs.

Once the RNG is seeded, we will use the `Mutator::RNG::getDouble(double min, double max)` method to generate random values within specified ranges. For example, we can set the angle to be randomly initialized between -π and π radians, and the angular velocity to be randomly initialized between -1.0 and 1.0 radians per second.

{% details Solution to #2 (Click to expand) %}
The reset method be modified as follows:
```cpp
/* pendulum_wrapper.cpp */
void PendulumWrapper::reset(size_t seed, Learn::LearningMode mode, uint16_t iterationNumber, uint64_t generationNumber) {
    // Seed the RNG differently for each iteration
	this->rng.setSeed(seed);

    // Randomize the initial angle between [-pi, pi]
	double initialAngle = this->rng.getDouble(-M_PI, M_PI);
	this->pendulum.setAngle(initialAngle);
	// Randomize the initial velocity between [-1.0, 1.0]
	double initialVelocity = this->rng.getDouble(-1.0, 1.0);
	this->pendulum.setVelocity(initialVelocity);

    this->accumulatedReward = 0.0;
}
```

{% enddetails %}

### Configure multi-episode evaluation in params.json

### Edit params.json
To enable multi-episode evaluation during training, we need to modify the training parameters in the `params.json` file of the project.

#### TODO #3:
Edit the `/gegelati-tutorial/params.json` file to set the `nbEpisodesPerEval` parameter to a value greater than 1. This parameter specifies the number of episodes over which each agent will be evaluated during training. For this tutorial, set it to 5.

{% details Solution to #3 (Click to expand) %}
```json
{
    // Existing parameters...
    "nbEpisodesPerEval": 5,
    // Existing parameters...
}
```

{% enddetails %}

### Run training with multi-episode evaluation
With the modifications to the `PendulumWrapper` class and the updated training parameters, we are now ready to run the training process with multi-episode evaluation enabled. During training, each agent will be evaluated over 5 episodes, and the average fitness across these episodes will be used to guide the evolutionary process.

## 2. Validation phase at the end of each generation
To monitor potential overfitting during training, we can implement a validation phase at the end of each generation. This involves evaluating the surviving best fitting roots of the TPG on a separate validation set of episodes that are not used during the training phase. By comparing the performance of the best agents on the training episodes and the validation episodes, we can detect if the agents are overfitting to the training data.

### Initialize validation episodes with fixed conditions

While the training episodes are randomly initialized at the beginning of each episode, the validation episodes can be initialized with a fixed set of starting conditions to ensure consistency in the evaluation.

To implement the validation phase, we will modify the `reset(size_t seed, Learn::LearningMode mode, uint16_t iterationNumber, uint64_t generationNumber)` method of the `PendulumWrapper` class to support a validation mode that uses fixed starting conditions for the pendulum's angle and angular velocity. The `mode` parameter provided to the `reset(...)` method can be used to determine whether the environment is in training mode or validation mode.

#### TODO #4:
Edit the `reset(size_t seed, Learn::LearningMode mode, uint16_t iterationNumber, uint64_t generationNumber)` method of the `PendulumWrapper` seed the environment's with a seed solely depending on the iteration number (but not the generation number) when the `mode` parameter is set to `Learn::LearningMode::VALIDATION`. This ensures that the validation episodes are initialized with fixed starting conditions for each iteration. Keep the random initialization for training episodes as previously implemented.

{% details Solution to #4 (Click to expand) %}
```cpp
void PendulumWrapper::reset(size_t seed, Learn::LearningMode mode, uint16_t iterationNumber, uint64_t generationNumber)
{
	if (mode == Learn::LearningMode::TRAINING) {
		// Seed the RNG differently for each iteration
		this->rng.setSeed(seed);
	}
	else {
		// In VALIDATION mode, use fixed seeds for reproducibility
		this->rng.setSeed(iterationNumber);
	}	

	// Randomize the initial angle between [- pi, pi]
	double initialAngle = this->rng.getDouble(-M_PI, M_PI);
	this->pendulum.setAngle(initialAngle);
	// Randomize the initial velocity between [-1.0, 1.0]
	double initialVelocity = this->rng.getDouble(-1.0, 1.0);
	this->pendulum.setVelocity(initialVelocity);

    this->accumulatedReward = 0.0;
}
```

{% enddetails %}

The remainder of this tutorial is in development.{: .notice--info}

## Conclusion
In this tutorial, you have successfully enabled multi-episode evaluation for reinforcement learning agents in <span style="font-variant: small-caps;">Gegelati</span>. By evaluating agents over multiple episodes, you have strengthened the robustness of the learned policy and mitigated the effects of randomness in the environment.

More information about reinforcement learning with <span style="font-variant: small-caps;">Gegelati</span> can be found in the following publication: 

[_K. Desnos, N. Sourbier, P.-Y. Raumer, O. Gesny and M. Pelcat. GEGELATI: Lightweight Artificial Intelligence through Generic and Evolvable Tangled Program Graphs. In Workshop on Design and Architectures for Signal and Image Processing (DASIP), ACM, 2021_](https://arxiv.org/pdf/2012.08296)

Since the training time of the learning agents were increased due to the multi-episode evaluation, the [the parallelization of the training process, covered in the linked tutorial, can be used to reduce the overall training time](/gegelati-tutorial/tutos/parallel-training).