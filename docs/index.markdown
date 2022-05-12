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
Compatibility of this tutorial was tested with MS Visual Studio Community Edition 2019, and GCC v9. 

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
Reboot your system at the end of the installation.
</details>

### Third-Party Library
A few third party libraries need to be installed 

## 1. Meet the pendulum

## 2. Interfacing with GEGELATI

## 3. Train your first TPG

## 4. The fun is only beginning.