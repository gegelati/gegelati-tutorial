cmake_minimum_required(VERSION 3.19.0)

# *******************************************
# ************* CMake Content ***************
# *******************************************
# This CMake create a workspace containing the following projects
# 
# Programs
#  - pendulum-tutorial

set (PROJECT_NAME pendulum-tutorial)

project(${PROJECT_NAME})

# Add definition for relative path into project
add_definitions( -DPROJECT_ROOT_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

# Disable C and C++ compiler extensions.
# C/CXX_EXTENSIONS are ON by default to allow the compilers to use extended
# variants of the C/CXX language.
# However, this could expose cross-platform bugs in user code or in the headers
# of third-party dependencies and thus it is strongly suggested to turn
# extensions off.
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio.*")

	# Link with pthread
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

	# Debug or release
	if(CMAKE_BUILD_TYPE MATCHES "Debug")
			MESSAGE("Generate Debug project")
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Debug)
			set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -pg -Wall")
	else()
			MESSAGE("Generate Release project")
			set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Release)
			set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
	endif()
	#add libmath during non visual studio builds
	set(CMAKE_EXTRA_LIB m)
else()
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Add definitions for testing purposes
if(${TESTING})
	MESSAGE("Testing mode")
	add_definitions(-DNO_CONSOLE_CONTROL -DNB_GENERATIONS=2)
endif()

# *******************************************
# *********** LIBRARIES and Dat *************
# *******************************************

set(DAT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dat)
include(${DAT_DIR}/CMakeLists.txt)
				
set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Add library (use include because gegelati doesn't support subdirectory yet)
include(${LIBS_DIR}/CMakeLists.txt)


# *******************************************
# ************** Executables  ****************
# *******************************************

# Common files for all projects
file(GLOB
	pendulum_files
	./src/*.cpp
	./src/*.h
)

# Sub project for manual control of the pendulum
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} "./src/")
add_executable(manual-control ${pendulum_files} "./src/manual/main-manual.cpp")
target_link_libraries(manual-control ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2TTF_LIBRARY})
target_compile_definitions(manual-control PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Sub project for the training of the TPG
file(GLOB
	training_files
	./src/training/*.cpp
	./src/training/*.h
	params.json
)
include_directories(${GEGELATI_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} "./src/")
add_executable(tpg-training ${pendulum_files} ${training_files})
target_link_libraries(tpg-training ${GEGELATI_LIBRARIES} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2TTF_LIBRARY})
target_compile_definitions(tpg-training PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Sub project for inference
file(GLOB
 inference_files
 ./src/inference/*.cpp
 ./src/inference/*.h
 ./src/training/instructions.*
 ./src/training/pendulum_wrapper.*
 params.json
)
include_directories(${GEGELATI_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} "./src/" "./src/training/")
add_executable(tpg-inference ${pendulum_files} ${inference_files})
target_link_libraries(tpg-inference ${GEGELATI_LIBRARIES} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2TTF_LIBRARY})
target_compile_definitions(tpg-inference PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")
