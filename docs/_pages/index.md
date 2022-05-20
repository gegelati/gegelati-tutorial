---
title: GEGELATI introductory tutorial
permalink: /
toc: true
toc_sticky: true
---

The objective of this tutorial is to guide you through the training of your first Tangled Program Graph (TPG) with the [<span style="font-variant: small-caps;">Gegelati</span>](https://github.com/gegelati/gegelati) library.
People following this tutorial are assumed to be already acquainted with basic structural and evolutionary concepts of TPGs.
In case you're not already familiar with these concepts, a basic introduction to these concepts and to <span style="font-variant: small-caps;">Gegelati</span> can be found in the [linked 10-minute video](https://youtu.be/t0Ta5Vo5h7s).

The following topics are covered in this tutorial:
* Configuration of a C++ project with [<span style="font-variant: small-caps;">Gegelati</span>](https://github.com/gegelati/gegelati).
* Manual interaction with a Reinforcement Learning (RL) environment.
* Interfacing of a RL environment with <span style="font-variant: small-caps;">Gegelati</span>.
* Training of a TPG.

This tutorial does not require any particular prerequisites.
While being fluent in C++ certainly is an asset to follow this tutorial, all pieces of codes that must be written during the tutorial are given in this assignment.

## 0. Setup and build the tutorial project

### C++ Environment:
This tutorial requires a C++ development environment compatible with the C++17 standard.
Compatibility of this tutorial was tested with MS Visual Studio Community Edition (MSVC) 2019, and GCC v9.

### Bash environment
Some scripts embedded in the given files of this tutorial require the availability of a bash environment.
While bash is natively supported in Linux and Mac OS, an installation is required on Windows.
We advise you to use the all-in-one binaries maintained by the git community, and available [here!](https://git-scm.com/downloads).
When installing the tool, make sure that the tool is added to the PATH environment variable.

### CMake
CMake is a utility tool whose purpose is to ease the portability of complex C/C++ applications by generating projects for most popular IDEs (CLion, Visual Studio, Makefile, QT Creator, ...), on major operating systems (Linux, Windows, Mac OS).
To achieve this purpose, source code files and project dependencies are specified in a configuration file, called `CMakeLists.txt`, using a specific description language.
When CMake is launched, it automatically generates a project for a specified IDE, where all dependencies to third-party libraries are configured.

CMake version 3.12 or newer must be installed for this tutorial.
To check if the CMake tool is already available on your workstation simply type the following command:
```bash
cmake --version

```
{% details In case CMake is not installed follow the click here to display installation advice. %}
The latest version of CMake can be downloaded at the following URL: https://cmake.org/download/.
For a simple installation, choose the binary version for windows.
During the installation process, select the "ADD TO PATH FOR ALL USERS" option.
Reboot your system at the end of the installation.
{% enddetails %}


### Third-Party Library
Several third party libraries need to be installed for this tutorial: <span style="font-variant: small-caps;">Gegelati</span>, `SDL2`, `SDL2_Image`, and `SDL2_ttf`.
The installation process for different OSes is given below.

{% details On Windows: (Click to expand) %}
All library binaries will be downloaded automatically when running the CMake project.
When using MSVC, all DLLs are copied automatically in the executable folders.
When using other compilers, if the library are not found during the build process, please refer to the [`/tutorial-gegelati/lib/ReadMe.md`](../lib/ReadMe.md) file for solutions.
{% enddetails %}

{% details On Linux: (Click to expand) %}
The SDL library (`SDL2`, `SDL2_Image`, and `SDL2_ttf`) are available in most package repository.
For example, on Ubuntu, simply type the following command:
```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev
```

To install <span style="font-variant: small-caps;">Gegelati</span>, you must build it and install it on your machine.
The following commands should do the work to build the latest release:
```bash
git clone -b master https://github.com/gegelati/gegelati.git
cd gegelati/bin
cmake ..
cmake --build . --target install # On Linux
```
{% enddetails %}

{% details On Mac OS: (Click to expand) %}
The SDL library (`SDL2`, `SDL2_Image`, and `SDL2_ttf`) are available in [Homebrew](https://brew.sh/).
Simply type the following command:
```bash
brew install sdl2 sdl2_image sdl2_ttf
```

To install <span style="font-variant: small-caps;">Gegelati</span>, you must build it and install it on your machine.
The following commands should do the work to build the latest release:
```bash
git clone -b master https://github.com/gegelati/gegelati.git
cd gegelati/bin
cmake ..
cmake --build . --target install # On Mac OS
```
{% enddetails %}

### Tutorial files
To download the source files needed to follow this tutorial, simply download the following archive: [gegelati-tutorial.zip](./data/gegelati-tutorial.zip).
Unzip these file in your favorite folder.
In the remainder of this tutorial, the parent folder where the files were unzipped will be referred as the `/gegelati-tutorial` folder.

To create the C++ project for your default build chain (Makefile on linux and Mac OS, MSVC -when installed- on windows), simply type the following commands in a bash terminal:
```bash
cd /gegelati-tutorial/bin
cmake ..
```
To select another IDE, use the `cmake -G "<MY_FAVORITE_IDE>" ..` command instead.
The list of available IDEs can be obtained by typing `cmake -G`.

Alternatively, some IDEs, like Clion, natively support CMake projects.
In this case, simply open a new project in the `/gegelati-tutorial` folder.

In all cases, make sure that no error appears during the generation of the CMake project.


## 1. Meet the pendulum
The learning environment used in this tutorial is an inverted pendulum.

<div align=center><img src="./img/pendulum_g6_200x200.gif" width="200" /></div>

The pendulum itself is a physical simulation of a weight attached to a pivot with a rigid cord, and subject to both gravity and friction.
In this learning environment, it is possible to apply a clockwise or a counter clockwise torque to the pendulum to influence its swinging.

As illustrated in the previous GIF, the objective of learning agent trained within this learning environment is to stabilize the pendulum in the upward position.
It is important to note that the maximum torque that can be applied to the pendulum is not strong enough to bring the pendulum to the upward position.
Hence, the only way to bring the pendulum to this position is to progressively gain some momentum with accelerated swings.

### Build and swing!
To better appreciate the difficulty of this task, the first learning agent trained in this tutorial relies on a well known machine learning technique: your brain!

To build the first executable of this tutorial, open the project generated with CMake; then build and run the `manual-control` executable.

Once the build is complete, run the executable.
Using the keyboard, you can try to manually balance the pendulum with 7 different actions:
* [S], [D], [F]: Apply a clockwise torque to the pendulum. The strength of the torque applied to the pendulum decreases from left to right.
* [J], [K], [L]: Apply a counter-clockwise torque to the pendulum. The strength of the torque applied to the pendulum increases from left to right.
* No key: No torque is applied to the pendulum. Only gravity and friction will do their work.

Good luck holding the pendulum in the upward position for more than a few seconds.

### Under the C++-hood
The simulation parameters of the pendulum can be changed when instantiating the corresponding C++ class.
The default parameter values were carefully chosen to give human beings a chance at stabilizing the pendulum.
Feel free to try other configurations by parameterizing the construction of the pendulum, in the `/gegelati-tutorial/src/manual/main-manual.cpp`

{% details `Pendulum` constructor documentation (Click to expand) %}
```cpp
/**
* \brief Default constructor for a pendulum.
*
* \param[in] a the initial angular position of the pendulum. Default value
* is M_PI, that is downard position.
* \param[in] c the initial angular velocity of the pendulum. Default value
* is 0.0.
* \param[in] maxSpeed Maximum speed of the pendulum
* \param[in] maxTorque maximum torque applicable to the pendulum.
* \param[in] timeDelta Simulation step duration.
* \param[in] gravity Universal constant
* \param[in] mass Weight of the pendulum.
* \param[in] length Length of the pendulum.
* \param[in] friction Friction slowing down the pendulum proportionally to
* its velovity.
*/
Pendulum(double a = M_PI, double v = 0.0, double maxSpeed = 16.0,
  double maxTorque = 1.8, double timeDelta = 0.01, double gravity = 9.81,
  double mass = 0.8, double length = 1.0, double friction = 0.005);
```

{% enddetails %}

## 2. Interfacing with GEGELATI

To train a Tangled Program Graph with <span style="font-variant: small-caps;">Gegelati</span>, the library needs to interact with the learning environment.
To achieve this purpose, the learning environment must be presented to the library using a standardized API.
In the case of <span style="font-variant: small-caps;">Gegelati</span>, this interfacing is done by specializing the `Learn::LearningEnvironment` abstract class.

<!--
|          LearningEnvironment       |
|:-----------------------------------|
|+getNbActions(): int                |
|+getDataSources(): std::vector<>    |
|+reset(): void                      |
|+doAction(int): void                |
|+getScore(): double                 |
|+isTerminal(): bool                 |
-->

The following steps will guide you through the creation of a `PendulumWrapper` class, inheriting from the `Learn::LearningEnvironment` class and interfacing the `Pendulum` class with <span style="font-variant: small-caps;">Gegelati</span>.

### `PendulumWrapper` skeleton.
The `pendulum_wrapper.h` and `pendulum_wrapper.cpp` files already contain the skeleton of the `PendulumWrapper` class, which you'll complete throughout the next steps.
To make the class compilable, this code already defines empty methods overriding all the pure virtual methods from the `Learn::LearningEnvironment`.
Comments in the header file explain the purpose of each method.

### `Pendulum` attributes
A first specific attribute of the `PendulumWrapper` class is already declared: the `actions` vector.
```cpp
/* From pendulum_wrapper.h */
/// Array containing the actions available for the TPG.
/// These actions are expressed as real numbers in [-1, 1], and will be multiplied
/// by the MAX_TORQUE of the Pendulum to apply the corresponding action.
static const std::vector<double> actions;
```

#### TODO #1:
Your first task is to update the definition of this vector in the `pendulum_wrapper.cpp` file, so that the 7 actions available to you in the manual version are also the one made available to the TPG.
It should be noted that the size of this `vector` is automatically used in the `PendulumWrapper` constructor to initialize the `LearningEnvironment` parent class with the number of actions available.
A single line of code needs to me modified in this task.

{%details Solution to #1 %}
```cpp
const std::vector<double> PendulumWrapper::actions{ -1.0, -0.66, -0.33, 0.0, 0.33, 0.66, 1.0 };
```
{% enddetails %}

#### TODO #2:
Add an instance of the `Pendulum` class to the attributes of the `PendulumWrapper` class.
Don't forget to include the `pendulum.h` file and update the constructor of the class to initialize the `Pendulum` instance, keeping default parameters for now.

{%details Solution to #2 %}
```cpp
/* pendulum_wrapper.h: After other includes */
#include <pendulum.h>
```
```cpp
/* pendulum_wrapper.h: After actions declaration */
  /// Pendulum interfaced with the GEGELATI Lib  
  Pendulum pendulum;
```
```cpp
/* pendulum_wrapper.cpp*/
PendulumWrapper::PendulumWrapper() : LearningEnvironment(actions.size(), pendulum())
{
}
```
{% enddetails %}

### Data access
In this step, you will expose the angle and velocity attributes of the pendulum so that they can be observed by the <span style="font-variant: small-caps;">Gegelati</span> library.

During the training process, the <span style="font-variant: small-caps;">Gegelati</span> library accesses the data from the environment using the `getDataSources()` method.
This method returns the observable state of the environment as a vector of references to `Data::DataHandler`.

The `Data::DataHandler` interface class provides a set of services that simplifies the training of TPGs.
In particular, in addition to methods for accessing the data, the `Data::DataHandler` supports methods for dynamically checking what the addressing space of a data source is, or which data types can be provided by a data source.
It is also possible for a `Data::DataHandler` to give access to data with a data type differing from the native type used for storing the data.

For example, with a `Primitive2DArray<char>(10,10)` data handler storing a 2D array of `char[10][10]`, individual elements of the array can be accessed using the native data type `char`; but it is also possible to access a 3-by-2 subregion of data by requesting an access to data with type `char[3][2]`.
For more details on these features, please refer to the doxygen documentation built and delivered with the <span style="font-variant: small-caps;">Gegelati</span> library.

In the case of the pendulum, we will use the `Data::PointerWrapper<T>` specialization of the `Data::DataHandler` class, which acts as an interface between a native pointer (`T *`) and <span style="font-variant: small-caps;">Gegelati</span>.
Only the following 2 methods of this class needs to be used in this tutorial:

```cpp
/**
 *  \brief Constructor for the PointerWrapper class.
 *
 * \param[in] ptr the pointer managed by the PointerWrapper.
 */
PointerWrapper(T* ptr = nullptr);

/**
 * \brief Set the pointer of the PointerWrapper.
 *
 * \param[in] ptr the new pointer managed by the PointerWrapper.
 *
 */
void setPointer(T* ptr);
```

#### TODO #3:
Instantiate two instances of the `Data::PointerWrapper` class as attributes of the `PendulumWrapper`.
In the constructor of the `PendulumWrapper`, make these two `Data::PointerWrapper` point to the `angle` and `velocity` attributes of the `pendulum`.
Then, update the `getDataSources()` method to return a vector referring to these two `Data::PointerWrapper`.
This task requires less than 10 lines of C++ code.

_C++ tip:_ Pushing a variable `T var` to a `std::vector<std::references_wrapper<T>> vect` is done with a simple call to `vect.push_back(var)`.

{% details Solution to #3 (Click to expand) %}

This solution is based on a vector of `Data::PointerWrapper<double>`.
Alternative solutions based on two `Data::PointerWrapper<double>` are perfectly viable.

```cpp
/* pendulum_wrapper.h : After pendulum declaration */
/// DataHandler wrappers
std::vector<Data::PointerWrapper<double>> data;
```
```cpp
/* pendulum_wrapper.cpp */
PendulumWrapper::PendulumWrapper() : LearningEnvironment(actions.size()), pendulum(), data(2)
{
	data.at(0).setPointer(&this->pendulum.getAngle());
	data.at(1).setPointer(&this->pendulum.getVelocity());
}

std::vector<std::reference_wrapper<const Data::DataHandler>> PendulumWrapper::getDataSources()
{
	std::vector<std::reference_wrapper<const Data::DataHandler>> result;
	result.push_back(this->data.at(0));
	result.push_back(this->data.at(1));
	return result;
}
```
{% enddetails %}

### Actions on the pendulum
After exposing the pendulum attributes to the learning agent, this step will give it the ability to act on the pendulum.

The number of discrete actions that can be taken by the learning agent is given by the `getNbActions()` method from the learning environment.
The value returned by this method is already set when calling the constructor of the `LearningEnvironment` parent class of the `PendulumWrapper`.

To execute an action, the learning agent calls the `doAction(int)` method of the learning environment with an argument corresponding to the action to execute.

#### TODO #4
Implement the `PendulumWrapper::doAction(int)` method using the actions defined in the `actions` attribute.
To apply a torque to the pendulum, the `Pendulum::applyTorque(double)` method must be used.
```cpp
/**
* \brief Apply the given torque to the pendulum and advance the simulation.
*
* \param[in] torque the torque applied to the pendulum. If the given value
* is not within the [-MAX_TORQUE, MAX_TORQUE] interval, it will be
* saturated to the closest bound.
*/
void applyTorque(double torque);
```
Two lines of C++ code are sufficient for this task.

{% details Solution to #4 (Click to expand) %}
```cpp
/* pendulum_wrapper.cpp */
void PendulumWrapper::doAction(uint64_t actionID)
{
  	// Retrieve the torque corresponding to the ID
  	double torque = this->actions[actionID] * pendulum.MAX_TORQUE;

  	// Apply it to the pendulum
  	this->pendulum.applyTorque(torque);
}
```
{% enddetails %}

#### TODO #5
To train the TPG, the reinforcement learning process requires making many attempts to stabilize it in the upward position.
Between each attempt, the initial position of the pendulum is reset using the `reset()` method.
Implement this method so that the pendulum managed by the `PendulumWrapper` always starts in a downward static state.
Two lines of code are needed for this task.

At this point, the arguments of the `reset` methods can be ignored.
Their utility will be covered in a more advanced tutorial.

{% details Solution to #5 (Click to expand) %}
```cpp
/* pendulum_wrapper.cpp */
void PendulumWrapper::reset(size_t seed, Learn::LearningMode mode)
{
	this->pendulum.setAngle(M_PI);
	this->pendulum.setVelocity(0.0);
}
```
{% enddetails %}

### Reward mechanism
In this last step, you are going to implement the reward mechanism used by <span style="font-variant: small-caps;">Gegelati</span> to identify the best strategies for controlling the pendulum.

The objective of this learning environment is to steady the pendulum in the upward position.
While it is easy to measure the distance of the pendulum to the upward position at each simulation step, appreciating the steadiness of the pendulum in this position can only be achieved over time.
Hence, the computation of the reward will be accumulated in a `double` attribute of the `PendulumWrapper`.
At each simulation step $i$, this reward will be updated as follows:

<div align="center">
$$ reward_{0} = 0 $$
$$ reward_{i} = reward_{i-1} - (0.1 \cdot \theta^2 + 0.01 \cdot \omega^2 + 0.001\cdot \tau^2) $$
</div>

where $\theta$ is the angular distance to the upward position, $\omega$ is the velocity of the pendulum, and $\tau$ the torque applied to the pendulum.
Intuitively, the purpose of this equation is to minimize the angular distance to the upward position as a primary criteria, but also the velocity of the pendulum when reaching this position, and the torque applied to the pendulum to reach and stay in this position.

#### TODO #6
Implement the rewarding mechanism in the `PendulumWrapper` class by:
* Adding an `accumulatedReward` attribute.
* Updating this reward after each action in the `doAction(int)` method.
* Returning this reward in the `getScore()` method.
* Resetting this reward in the `reset(int, LearningMode)` method.
Less than 10 new lines of code are needed for this task.

_C++ tip:_ The `double fmod(double, double)` method can be used to compute the modulo of two `double` numbers.

{% details Solution to #6 (Click to expand) %}

```cpp
/* pendulum_wrapper.h : After the PointerWrapper */
	double accumulatedReward;
```

```cpp
/* pendulum_wrapper.cpp */
void PendulumWrapper::reset(size_t seed, Learn::LearningMode mode)
{
	this->pendulum.setAngle(M_PI);
	this->pendulum.setVelocity(0.0);

	this->accumulatedReward = 0.0;
}

void PendulumWrapper::doAction(uint64_t actionID)
{
	// Retrieve the torque corresponding to the ID
	double torque = this->actions[actionID] * pendulum.MAX_TORQUE;

	// Apply it to the pendulum
	this->pendulum.applyTorque(torque);

	// Get the angle value between -M_PI and M_PI (0 being the upward position)
	double angle = fmod(this->pendulum.getAngle() + M_PI, 2.0 * M_PI) - M_PI;

	// Compute a reward based on the angle to the upward position, the velocity and the torque.
	// All three values should be minimized.
	double reward = -(0.1f*(angle * angle) + 0.01f * (this->pendulum.getVelocity() * this->pendulum.getVelocity()) + 0.001f * (torque * torque));

	// Accumulate the reward
	accumulatedReward += reward;
}

double PendulumWrapper::getScore(void) const
{
	return accumulatedReward;
}
```
{% enddetails %}

The `pendulum_wrapper.h` and `pendulum_wrapper.cpp` files resulting from this tutorial can be downloaded at the following link: [pendulum_wrapper_solution.zip](./data/pendulum_wrapper_solution.zip).

## 3. Train your first TPG
The code needed to interface the pendulum with <span style="font-variant: small-caps;">Gegelati</span> is now complete.
This section will (finally) let you train a TPG with the pendulum learning environment.

### Training procedure
The `main-training.cpp` file contains the entry point of the executable built with the `tpg-training` target.
The program is structured as follows:
1. Initialize the instruction set used in programs of the TPG.
2. Load the training meta-parameters from the `gegelati-tutorial/params.json` file.
3. Instantiate the `PendulumWrapper` learning environment.
4. Instantiate the `Learn::LearningAgent` class. This utility class will manage most aspects of the training process of the TPG, including its initialization, mutations, and evaluation of the fitness of its roots within the learning environment.
5. Initialize a display window. This display will be used throughout the training to show the behavior of the TPG root with the best score after each generation.
6. Instantiate a logger for keeping track of the training statistics throughout the training.
7. Iterate the genetic evolution process until the maximum number of generation is reached, or until the program is exited.
8. Train one generation. This step, managed by the learning agent, includes the mutation of the TPG to reach the desired number of roots, the evaluation of all the TPG roots, and the decimation of worst fitting root.
9. Create a replay of all actions performed by the best fitting root of the TPG, and trigger a refresh of the display.
10. Go back to step 7.

### Training in action
Build and run the `tpg-training` target to observe the TPG training process in action.

The first output of the training process are the logs generated in the console.
An example of log is presented hereafter:

```bash
Pendulum TPG training.
                      Train
      Gen   NbVert      Min      Avg      Max  T_mutat   T_eval  T_total
        0      164 -7624.63 -1383.36  -980.33     0.00     1.51     1.51
        1      167 -4848.23 -1263.68  -980.33     7.28     1.56    10.38
        2      170 -6862.46 -1242.03  -844.01     2.63     1.53    14.55
        3      170 -7707.18 -1181.00  -844.01     7.60     1.48    23.65
        4      168
```

The generated logs contain a table that can be exported in the CSV format by giving a file path to the `LABasicLogger` constructor.
This table contains the following columns:
* _Gen_: Number of generation since the start of the training process.
* _NbVert_: Number of vertices (teams + actions) in the TPG before the fitness of the roots is evaluated.
* _Train.Min/Avg/Max_: Minimum, average, and maximum scores obtained during the evaluation process of all roots of the TPG.
* _T\_mutat_: Time taken in seconds to apply the mutations to the TPG.
* _T\_eval_: Time taken  in seconds to evaluate all roots of the TPG.
* _T\_total_: Time  in seconds since the beginning of the training process.

A few insights on these logs:
* It may happen that the maximum score observed at generation $n+1$ is lower than the score observed at a previous generation $n$.
This phenomenon occurs when the best root vertex observed at generation $n$ becomes an internal team of the TPG during the mutation process creating new roots for the next generation.
While this may seem weird to see the score decrease, the genetic material that had lead to a better result is not lost.
If the new roots introduced in the graph do a poor job, they will be decimated rapidly, and the best root will once again become a root.
It may also happen that the former best root remains an internal vertex of the TPG, but is copied during the mutation process, thus becoming a slightly altered root once again.
In any case, the vertex which, as a root, has provided the best reward since the beginning of the training is still the one returned by the learning agent when requesting the best root with the `Learn::LearningAgent::getBestRoot()` method.
* In this learning environment, the time taken for mutating the graph is quite long, especially compared to the time taken for evaluating the roots.
The reason behind this abnormally long mutation times lies in a specific mechanism ensuring the originality of programs produced during the mutation process.
Each time a new program is created, it is compared to randomly selected pre-existing programs.
If the new program does not produce original outputs compared to other programs, it will be mutated over and over until its output becomes original.
While producing an original output is simple in learning environment with large observable space, this is a lot trickier to achieve in a learning environment with only 2 observable variables.
This is why the mutation time is long with this example.

The second output of the training process is the display of the pendulum.
While the score presented in the logs are not easily interpretable, this display makes it possible to appreciate how well the trained TPGs are doing.

With default pendulum parameters and meta-parameters, the learning agent should be able to stabilize the pendulum in less than 25 generations.
During the first generations, the learning agent usually learns how to swing the pendulum closer to the upward position.
Then it learns how to slow down the pendulum when it approaches the upward position.
Finally, it finds a way to stabilize the pendulum.

## 4. The fun is only beginning.
In this tutorial, you have learned:
* How to setup a project with <span style="font-variant: small-caps;">Gegelati</span>.
* Create a custom learning environment.
* Train TPG in the learning environment.
* Understand the basic logs generated during the training.

While this tutorial illustrates the basic concepts of TPG training, <span style="font-variant: small-caps;">Gegelati</span> offers many features to play with TPGs.
The following tutorials guide you through these features, starting from the result of this tutorial.
These tutorial can be followed in any order, so feel free to explore them based on your personal interests and objectives.
* _[Available Soon]_ Export, visualize and import TPG graphs in the DOT format.
* _[Available Soon]_ Accelerate training with early termination, random initial states and testing.
* _[Available Soon]_ Play with training meta-parameters.
* _[Available Soon]_ Explore TPG statistics throughout the training.
* _[Available Soon]_ Accelerate TPG training with deterministic multi-core support.
* _[Available Soon]_ Generate standalone C code for ultra-fast inference.

Several other open-source applications are available in the GitHub repository of <span style="font-variant: small-caps;">Gegelati</span>.
Feel free to explore them to get a better understanding of the wonderful abilities of TPGs.
* [<span style="font-variant: small-caps;">Gegelati</span>-apps](https://github.com/gegelati/gegelati-apps): MNIST, Tic-Tac-Toe, Nim Game
* [ALE-wrapper](https://github.com/gegelati/ale-wrapper): Wrapper for the 50+ games of the Arcade Learning Environment.
* [ArmLearn-wrapper](https://github.com/gegelati/armlearn-wrapper): Robotic arm control with 6 servo-motors.
* [Gym-<span style="font-variant: small-caps;">Gegelati</span>](https://github.com/gegelati/gym-gegelati): Wrapper for OpenAI's Gym reinforcement learning dataset.
