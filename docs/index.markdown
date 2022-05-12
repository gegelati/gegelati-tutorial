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
<details><summary>In case CMake is not installed follow the click here to display installation advice.</summary>
The latest version of CMake can be downloaded at the following URL: https://cmake.org/download/. 
For a simple installation, choose the binary version for windows. 
During the installation process, select the "ADD TO PATH FOR ALL USERS" option. 
Reboot your system at the end of the installation.</details>


### Third-Party Library
Several third party libraries need to be installed for this tutorial: <span style="font-variant: small-caps;">Gegelati</span>, `SDL2`, `SDL2_Image`, and `SDL2_ttf`. 
The installation process for different OSes is given below.

<details><summary>On Windows</summary>
All library binaries will be downloaded automatically when running the CMake project.
When using MSVC, all DLLs are copied automatically in the executable foldere.
When using other compilers, if the library are not found during the build process, please refer to the [`/tutorial-gegelati/lib/ReadMe.md`](../lib/ReadMe.md) file for solutions.</details>

<details><summary>On Linux</summary>
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
```</details>


### Tutorial files
To download the source files needed to follow this tutorial, simply download the following archive: [TODO](TODO).
Unzip these file in your favorite folder.
In the reminder of this tutorial, the parent folder where the files were unzipped will be refered as the `/gegelati-tutorial` folder.

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

## 2. Interfacing with GEGELATI

## 3. Train your first TPG

## 4. The fun is only beginning.