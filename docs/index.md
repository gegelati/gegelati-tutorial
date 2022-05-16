---
# Feel free to add content and custom Front Matter to this file.
# To modify the layout, see https://jekyllrb.com/docs/themes/#overriding-theme-defaults
title: GEGELATI introductory tutorial
layout: home
---

The objective of this tutorial is to gently guide you through the training of your first Tangled Program Graph (TPG) with the [<span style="font-variant: small-caps;">Gegelati</span>](https://github.com/gegelati/gegelati) library.
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
While this is natively supported in Linux, an installation is required on Windows.
We advise you to use the all-in-one binaries maintained by the git community, and available [here!](https://git-scm.com/downloads).
When installing the tool, make sure that the tool is added to the PATH environment variable.

### CMake
CMake is a utility tool whose purpose is to ease the portability of complex C/C++ applications by generating projects for most popular IDEs (Code::Blocks, Visual Studio, Makefile, QT Creator, ...), on major operating systems (Linux, Windows, Mac OS).
To achieve this purpose, source code files and project dependencies are specified in a configuration file, called `CMakeLists.txt`, using a specific description language.
When CMake is launched, it automatically generates a project for a specified IDE, where all dependencies to third-party libraries are configured.

To check if the CMake tool is already available on your workstation simply type the following command:
```bash
cmake --version
```
CMake version 3.12 or newer must be installed for this tutorial.
{% details In case CMake is not installed follow the click here to display installation advice. %}
The latest version of CMake can be downloaded at the following URL: https://cmake.org/download/.
For a simple installation, choose the binary version for windows.
During the installation process, select the "ADD TO PATH FOR ALL USERS" option.
Reboot your system at the end of the installation.
{% enddetails %}


### Third-Party Library
Several third party libraries need to be installed for this tutorial: <span style="font-variant: small-caps;">Gegelati</span>, `SDL2`, `SDL2_Image`, and `SDL2_ttf`.
The installation process for different OSes is given below.

{% details On Windows: %}
All library binaries will be downloaded automatically when running the CMake project.
When using MSVC, all DLLs are copied automatically in the executable foldere.
When using other compilers, if the library are not found during the build process, please refer to the [`/tutorial-gegelati/lib/ReadMe.md`](../lib/ReadMe.md) file for solutions.
{% enddetails %}

{% details On Linux: %}
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


### Tutorial files
To download the source files needed to follow this tutorial, simply download the following archive: [TODO](TODO).
Unzip these file in your favorite folder.
In the reminder of this tutorial, the parent folder where the files were unzipped will be referred as the `/gegelati-tutorial` folder.

To create the C++ project for your default build chain (Makefile on linux, MSVC -when installed- on windows), simply type the following commands in a bash terminal:
```bash
cd /gegelati-tutorial/bin
cmake ..
```
To select another IDE, use the `cmake -G "<MY_FAVORITE_IDE>" ..` command instead.
The list of available IDE can be obtained by typing `cmake -G`.

Alternatively, some IDEs, like Clion, natively support CMake project.
In this case, simply open a new project in the `/gegelati-tutorial` folder.

In all cases, make sure that no error appears during the generation of the CMake project.


## 1. Meet the pendulum
The learning environment used in this tutorial is an inverted pendulum.

<div align=center><img src="./img/pendulum_g6_200x200.gif" width="200" /></div>

The pendulum itself is a physical simulation of a weight attached to a pivot with a rigid cord, and subject to both gravity and friction.
In this learning environment, it is possible to apply a clockwise or a counter clockwise torque to the pendulum to influence its swinging.

As illustrated in the previous GIF, the objective of learning agent trained within this learning environment is to stabilize the pendulum in the upward position.
It is important to note that the maximum torque that can be applied to the pendulum is not strong enough to bring the pendulum to the upward position.
Hence, the only way to bring the pendulum to this position is to progressivelly gain some momentum with accelerated swings.

### Build and swing!
To better appreciate the difficulty of this task, the first learning agent trained in this tutorial relies on a well known machine learning technique: your brain!

To build the first executable of this tutorial, open the project generated with CMake; then build and run the `manual-control` executable.

Once the build is complete, run the executable.
Using the keyboard, you can try to manually balance the pendulum with 7 different actions:
* [C], [V], [B]: Apply a clockwise torque to the pendulum. The strength of the torque applied to the pendulum decreases from left to right.
* [1], [2], [3]: Apply a counter-clockwise torque to the pendulum. The strength of the torque applied to the pendulum increases from left to right.
* No key: No torque is applied to the pendulum. Only gravity and friction will do their work.

Good luck holding the pendulum in the upward position for more than a few seconds.

### Under the C++-hood
The simulation parameters of the pendulum can be changed when instantiating the corresponding C++ class.
The default parameter values were carefully chosen to give human beings a chance at stabilizing the pendulum.
Feel free to try other configurations by parameterizing the construction of the pendulum, in the `/tutorial-gegelati/src/manual/main-manual.cpp`

{% details `Pendulum` constructor documentation %}
```cpp
/**
* \brief Default constructor for a pendulum.
*
* \param[in] a the initial angular position of the pendulum. Default value
* is 0.0, that is downard position.
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
  double maxTorque = 2.0, double timeDelta = 0.01, double gravity = 9.81,
  double mass = 0.8, double length = 1.0, double friction = 0.005);
```

{% enddetails %}

## 2. Interfacing with GEGELATI

To train a Tangled Program Graph with <span style="font-variant: small-caps;">Gegelati</span>, the library needs to interact with the learning environment.
To achieve this purpose, the learning environment must be presented to the library using a standardized API.
In the case of <span style="font-variant: small-caps;">Gegelati</span>, this interfacing is done by specializing the `Learn::LearningEnvironment` class.

|          LearningEnvironment       |
|:-----------------------------------|
|+getNbActions(): int                |
|+getDataSources(): std::vector<>    |
|+reset(): void                      |
|+doAction(int): void                |
|+getScore(): double                 |
|+isTerminal(): bool                 |


The following step will guide you through the creation of a `PendulumWrapper` class, inheriting from the `Learn::LearningEnvironment` class and interfacing the `Pendulum` class with <span style="font-variant: small-caps;">Gegelati</span>.


## 3. Train your first TPG

## 4. The fun is only beginning.
