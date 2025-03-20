# Guideline for Libraries Installation
## Content 

This file contains instructions to setup the libraries in order to compile the
rendering part of the pendulum project. 

Instructions have been tested for:

* Windows 7 
  * MinGW
  * Visual Studio 2022
  * CLion
  
The project compilation requires the following libraries:

* SFML 3.0

## SFML
Latest version of the library should be downloaded of (better) built from source, following instructions from the [Official website](https://www.sfml-dev.org/tutorials/3.0/getting-started/cmake/).
When building the library with CMake, make sure to use the shared version of the lib:
```bash
cmake .. -DBUILD_SHARED_LIBS=ON
```
