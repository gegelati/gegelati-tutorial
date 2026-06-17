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
Edit the `/gegelati-tutorial/src/environments/pendulum_wrapper.h` to add a random number generator as a member variable of the `PendulumWrapper` class. This pseudo-random number generator is provided in <span style="font-variant: small-caps;">Gegelati</span> with the `Mutator::RNG` class.

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

The output of the training process generated in the console should look as follows:
```bash
                                                 Train
      Gen   NbVert   NbActR  NbTeamR      Min      Avg      Max  T_eval  T_decim  T_mutat  T_total
        0      157        0      150 -7315.27 -1746.27  -839.34    6.67     0.00     3.43    10.10
        1      160        0      150 -5556.83 -1469.08  -644.54    9.70     0.00     4.68    24.48
        2      159        0      150 -4979.20 -1274.80  -744.56    9.15     0.00     5.26    38.89
        3      161        0      150 -3757.80 -1268.95  -744.56    9.68     0.00     5.84    54.41
```

The cumulative execution time should be approximately 5 times longer than before. The reported `Max`, `Avg`, and `Min` training scores should also change. In this specific learning environment, the diversity of initial conditions in the multi-episode evaluation makes the learning task slightly easier at the beginning of the process, resulting in a higher `Max` score and slightly faster training of the agent in the initial generations. Indeed, with randomized initial angle and velocity, the pendulum will already be swinging in most episodes, which on average improves its proximity to the desired upward position, thus automatically increasing the received reward.

## 2. Validation phase at the end of each generation
When training an agent with a varying environments, where the difficulty of the task can change from one episode to another, it is possible that the agent overfits to the training episodes and fails to generalize to unseen situations. For example, in the pendulum environment, an agent may learn to balance the pendulum only when it starts from a specific angle and velocity, but fail to do so when the initial conditions are different.

To monitor potential overfitting during training, we can implement a validation phase at the end of each generation. This involves evaluating the surviving best fitting roots of the TPG on a separate validation set of episodes that are not used during the training phase. By comparing the performance of the best agents on the training episodes and the validation episodes, we can detect if the agents are overfitting to the training data.

### Initialize validation episodes with fixed conditions

While the training episodes are randomly initialized at the beginning of each episode, the validation episodes can be initialized with a fixed set of starting conditions to ensure consistency in the evaluation.

To implement the validation phase, we will once again modify the `reset(size_t seed, Learn::LearningMode mode, uint16_t iterationNumber, uint64_t generationNumber)` method of the `PendulumWrapper` class to support a validation mode that uses fixed starting conditions for the pendulum's angle and angular velocity. The `mode` parameter provided to the `reset(...)` method can be used to determine whether the environment is in training mode or validation mode.

#### TODO #4:
Edit the `reset(size_t seed, Learn::LearningMode mode, uint16_t iterationNumber, uint64_t generationNumber)` method of the `PendulumWrapper`. When the `mode` parameter is not set to `Learn::LearningMode::TRAINING`, seed the environment's random number generator with a seed solely depending on the `iterationNumber` (but not the `generationNumber` nor the given `seed`). This ensures that the validation episodes are initialized with fixed starting conditions for each iteration of the validation epidodes. Keep the random initialization for training episodes as previously implemented.

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

### Activate validation phase in params.json
To enable the validation phase at the end of each generation, we need to modify the training parameters in the `params.json` file of the project.

#### TODO #5:
Edit the `/gegelati-tutorial/params.json` file to set the `doValidation` parameter to a `true`.

{% details Solution to #5 (Click to expand) %}
```json
{
    // Existing parameters...
    "doValidation": true,
    // Existing parameters...
}
```

{% enddetails %}

### Run training with validation phase
With the modifications to the `PendulumWrapper` class and the updated training parameters, we are now ready to run the training process with the validation phase enabled. During training, after each generation, the best fitting roots of the TPG will be evaluated on a separate validation set of episodes, and the average fitness on the validation episodes will be reported in the console output. 

```bash
Pendulum TPG training.
                                                 Train                      Valid
      Gen   NbVert   NbActR  NbTeamR      Min      Avg      Max      Min      Avg      Max   T_eval  T_valid  T_decim  T_mutat  T_total
        0      157        0      150 -7315.27 -1746.27  -839.34 -1515.04 -1277.94  -858.22     5.99     3.07     0.00     3.12     9.12
        1      160        0      150 -5556.83 -1469.08  -644.54 -1381.13 -1215.51  -858.22     8.90     4.22     0.00     4.26    22.28
        2      159        0      150 -4979.20 -1274.80  -744.56 -1293.25 -1198.73  -858.22     8.41     4.78     0.00     4.84    35.53
        3      161        0      150 -3757.80 -1268.95  -744.56 -1362.07 -1192.69  -858.22     8.91     5.30     0.00     5.37    49.81
```

As expected, new columns are added to the console output to report the minimum, average, and maximum fitness of the best fitting roots of the TPG on the validation episodes. The reported fitness values are different from those on the training episodes, as different initial conditions are used for the validation episodes. In general, the reported fitness values on the validation episodes should be lower, but not too far from those on the training episodes especially for the maximum fitness. Divergence between the training and validation fitness values may indicate that the agents are overfitting to the training episodes and failing to generalize to unseen situations.

An additional column is also added to report the execution time of the validation phase, which should be shorter than the execution time of the training phase, as only the best fitting roots of the TPG are evaluated on the validation episodes, instead of all the agents in the population.

## Conclusion
In this tutorial, you have successfully enabled multi-episode evaluation for reinforcement learning agents in <span style="font-variant: small-caps;">Gegelati</span>. By evaluating agents over multiple episodes, you have strengthened the robustness of the learned policy and mitigated the effects of randomness in the environment.

More information about reinforcement learning with <span style="font-variant: small-caps;">Gegelati</span> can be found in the following publication: 

[_K. Desnos, N. Sourbier, P.-Y. Raumer, O. Gesny and M. Pelcat. GEGELATI: Lightweight Artificial Intelligence through Generic and Evolvable Tangled Program Graphs. In Workshop on Design and Architectures for Signal and Image Processing (DASIP), ACM, 2021_](https://arxiv.org/pdf/2012.08296)

Since the training time of the learning agents were increased due to the multi-episode evaluation, the [the parallelization of the training process, covered in the linked tutorial, can be used to reduce the overall training time](/gegelati-tutorial/tutos/parallel-training).