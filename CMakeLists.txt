cmake_minimum_required(VERSION 3.12.4)

# *******************************************
# ************* CMake Content ***************
# *******************************************
# This CMake create a workspace containing the following projects
# 
# Programs
#  - pendulum-tutorial

set (PROJECT_NAME pendulum-tutorial)

project(${PROJECT_NAME})

set(GEGELATI_VERSION "1.2.0")
set(GEGELATI_ARCHIVE "gegelatilib-${GEGELATI_VERSION}.zip")
set(GEGELATI_URL "https://github.com/gegelati/gegelati/releases/download/v${GEGELATI_VERSION}/${GEGELATI_ARCHIVE}")

set(SDL2_VERSION "2.28.5")
set(SDL2_MINGW_ARCHIVE "SDL2-devel-${SDL2_VERSION}-mingw.zip")
set(SDL2_VC_ARCHIVE "SDL2-devel-${SDL2_VERSION}-VC.zip")
set(SDL2_MINGW_URL "https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/${SDL2_MINGW_ARCHIVE}")
set(SDL2_VC_URL "https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/${SDL2_VC_ARCHIVE}")

set(SDL2_TTF_VERSION "2.20.2")
set(SDL2_TTF_MINGW_ARCHIVE "SDL2_ttf-devel-${SDL2_TTF_VERSION}-mingw.zip")
set(SDL2_TTF_VC_ARCHIVE "SDL2_ttf-devel-${SDL2_TTF_VERSION}-VC.zip")
set(SDL2_TTF_MINGW_URL "https://github.com/libsdl-org/SDL_ttf/releases/download/release-${SDL2_TTF_VERSION}/${SDL2_TTF_MINGW_ARCHIVE}")
set(SDL2_TTF_VC_URL "https://github.com/libsdl-org/SDL_ttf/releases/download/release-${SDL2_TTF_VERSION}/${SDL2_TTF_VC_ARCHIVE}")

set(SDL2_IMAGE_VERSION "2.8.0")
set(SDL2_IMAGE_MINGW_ARCHIVE "SDL2_image-devel-${SDL2_IMAGE_VERSION}-mingw.zip")
set(SDL2_IMAGE_VC_ARCHIVE "SDL2_image-devel-${SDL2_IMAGE_VERSION}-VC.zip")
set(SDL2_IMAGE_MINGW_URL "https://github.com/libsdl-org/SDL_image/releases/download/release-${SDL2_IMAGE_VERSION}/${SDL2_IMAGE_MINGW_ARCHIVE}")
set(SDL2_IMAGE_VC_URL "https://github.com/libsdl-org/SDL_image/releases/download/release-${SDL2_IMAGE_VERSION}/${SDL2_IMAGE_VC_ARCHIVE}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/lib/cmake_modules/")


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

MESSAGE("Download dat content.")
execute_process(COMMAND sh -c ${CMAKE_SOURCE_DIR}/dat/download_dat.sh
				WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/dat/)

set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)				

# *******************************************
# *********** GEGELATI LIBRARY **************
# *******************************************

if(WIN32)

	# find the Gegelati-x.y.z folder in the lib directory.
	file(GLOB GEGELATI_ROOT_DIR "${LIBS_DIR}/gegelatilib-[\\.|0-9]*")
	set(ENV{GEGELATI_DIR} ${GEGELATI_ROOT_DIR})

	# if SDL lib folder exists
	if(GEGELATI_ROOT_DIR MATCHES "$^")

		if (NOT MSVC)
			message(SEND_ERROR "Non-MSVC build on windows is not supported automatically. Please build the lib by yourself.")
		endif()

		message("Downloading GEGELATI library from ${GEGELATI_URL}")
		file(DOWNLOAD ${GEGELATI_URL} "${LIBS_DIR}/${GEGELATI_ARCHIVE}")

		message("Extracting GEGELATI library")
		file(ARCHIVE_EXTRACT
				INPUT "${LIBS_DIR}/${GEGELATI_ARCHIVE}"
				DESTINATION ${LIBS_DIR})
		file(REMOVE ${LIBS_DIR}/${GEGELATI_ARCHIVE})

		file(GLOB GEGELATI_ROOT_DIR "${LIBS_DIR}/gegelatilib-[\\.|0-9]*")
		set(ENV{GEGELATI_DIR} ${GEGELATI_ROOT_DIR})

	endif ()

endif()

find_package(GEGELATI REQUIRED)

if(NOT GEGELATI_FOUND)
	MESSAGE(FATAL_ERROR "GEGELATI not found !")
endif()

if (WIN32)
	file(GLOB
			GEGELATI_DLL
			${GEGELATI_ROOT_DIR}/bin/*.dll
			${GEGELATI_ROOT_DIR}/lib/*.dll
			)

	MESSAGE("Copy GEGELATI DLLs into ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
	file(COPY ${GEGELATI_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()


# *******************************************
# ************** SDL2 LIBRARY ***************
# *******************************************
# Set variables before finding SDL Library
# Helps to find where SDL libraries are saved 
if(WIN32)

	# find the SDL2-x.y.z folder in the lib directory.
	file(GLOB SDL2DIR "${LIBS_DIR}/SDL2-[\\.|0-9]*")
	set(ENV{SDL2DIR} ${SDL2DIR})

	# if SDL lib folder exists
	if(SDL2DIR MATCHES "$^")

		if (MSVC)
			set(SDL2_URL ${SDL2_VC_URL})
			set(SDL2_ARCHIVE ${SDL2_VC_ARCHIVE})
		else ()
			set(SDL2_URL ${SDL2_MINGW_URL})
			set(SDL2_ARCHIVE ${SDL2_MINGW_ARCHIVE})
		endif ()

		message("Downloading SDL2 library from ${SDL2_URL}")
		file(DOWNLOAD ${SDL2_URL} "${LIBS_DIR}/${SDL2_ARCHIVE}")

		message("Extracting SDL2 library")
		file(ARCHIVE_EXTRACT
				INPUT "${LIBS_DIR}/${SDL2_ARCHIVE}"
				DESTINATION ${LIBS_DIR})
		file(REMOVE ${LIBS_DIR}/${SDL2_ARCHIVE})

		file(GLOB SDL2DIR "${LIBS_DIR}/SDL2-[\\.|0-9]*")
		set(ENV{SDL2DIR} ${SDL2DIR})

		# Moving files
		if (MSVC)
			file(COPY "${SDL2DIR}/lib/x64/" DESTINATION "${SDL2DIR}/lib/")
		else ()
			file(COPY "${SDL2DIR}/x86_64-w64-mingw32/bin/" DESTINATION "${SDL2DIR}/bin/")
			file(COPY "${SDL2DIR}/x86_64-w64-mingw32/lib/" DESTINATION "${SDL2DIR}/lib/")

			file(COPY "${SDL2DIR}/x86_64-w64-mingw32/include/SDL2/" DESTINATION "${SDL2DIR}/include/"
					FILES_MATCHING PATTERN "*.h")
		endif ()

		# Cleaning/Removing unused files
		# MinGW
		file(REMOVE_RECURSE "${SDL2DIR}/i686-w64-mingw32")
		file(REMOVE_RECURSE "${SDL2DIR}/x86_64-w64-mingw32")
		file(REMOVE_RECURSE "${SDL2DIR}/test")

		# MSVC
		file(REMOVE_RECURSE "${SDL2DIR}/lib/x86")
		file(REMOVE_RECURSE "${SDL2DIR}/lib/x64")

	endif ()

endif()

Find_Package (SDL2 REQUIRED )

if(NOT SDL2_FOUND)
	MESSAGE(FATAL_ERROR "SDL not found !")
endif()

if (WIN32)
	file(GLOB
			SDL2_DLL
			${SDL2DIR}/bin/*.dll
			${SDL2DIR}/lib/*.dll
			)

	MESSAGE("Copy SDL2 DLLs into ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
	file(COPY ${SDL2_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

# *******************************************
# ************ SDL2 IMAGE LIBRARY ***********
# *******************************************
# Set variables before finding SDL2IMAGE Library
# Helps to find where SDL2IMAGE libraries are saved
if(WIN32)
	# find the SDL2_image-x.y.z folder in the lib directory.
	file(GLOB SDL2_IMAGE_DIR "${LIBS_DIR}/SDL2_image-[\\.|0-9]*")

	# if SDL lib folder exists
	if(SDL2_IMAGE_DIR MATCHES "$^")

		if (MSVC)
			set(SDL2_IMAGE_URL ${SDL2_IMAGE_VC_URL})
			set(SDL2_IMAGE_ARCHIVE ${SDL2_IMAGE_VC_ARCHIVE})
		else ()
			set(SDL2_IMAGE_URL ${SDL2_IMAGE_MINGW_URL})
			set(SDL2_IMAGE_ARCHIVE ${SDL2_IMAGE_MINGW_ARCHIVE})
		endif ()

		message("Downloading SDL2 Image library from ${SDL2_IMAGE_URL}")
		file(DOWNLOAD ${SDL2_IMAGE_URL} "${LIBS_DIR}/${SDL2_IMAGE_ARCHIVE}")

		message("Extracting SDL2_IMAGE library")
		file(ARCHIVE_EXTRACT
				INPUT "${LIBS_DIR}/${SDL2_IMAGE_ARCHIVE}"
				DESTINATION ${LIBS_DIR})
		file(REMOVE ${LIBS_DIR}/${SDL2_IMAGE_ARCHIVE})

		file(GLOB SDL2_IMAGE_DIR "${LIBS_DIR}/SDL2_image-[\\.|0-9]*")
		

		# Moving files
		if (MSVC)
			file(COPY "${SDL2_IMAGE_DIR}/lib/x64/" DESTINATION "${SDL2_IMAGE_DIR}/lib/")
		else ()
			file(COPY "${SDL2_IMAGE_DIR}/x86_64-w64-mingw32/bin/" DESTINATION "${SDL2_IMAGE_DIR}/bin/")
			file(COPY "${SDL2_IMAGE_DIR}/x86_64-w64-mingw32/lib/" DESTINATION "${SDL2_IMAGE_DIR}/lib/")

			file(COPY "${SDL2_IMAGE_DIR}/x86_64-w64-mingw32/include/SDL2/" DESTINATION "${SDL2_IMAGE_DIR}/include/"
					FILES_MATCHING PATTERN "*.h")
		endif ()

		# Cleaning/Removing unused files
		# MinGW
		file(REMOVE_RECURSE "${SDL2_IMAGE_DIR}/i686-w64-mingw32")
		file(REMOVE_RECURSE "${SDL2_IMAGE_DIR}/x86_64-w64-mingw32")
		file(REMOVE_RECURSE "${SDL2_IMAGE_DIR}/test")

		# MSVC
		file(REMOVE_RECURSE "${SDL2_IMAGE_DIR}/lib/x86")
		file(REMOVE_RECURSE "${SDL2_IMAGE_DIR}/lib/x64")

	endif ()

	# Set variable for find_module
	set(ENV{SDL2IMAGEDIR} ${SDL2_IMAGE_DIR})

endif()

Find_Package (SDL2_image REQUIRED )

if(NOT SDL2IMAGE_FOUND)
	MESSAGE(FATAL_ERROR "SDL2IMAGE not found !")
endif()

if (WIN32)
	file(GLOB
			SDL2_IMAGE_DLL
			${SDL2_IMAGE_DIR}/bin/*.dll
			${SDL2_IMAGE_DIR}/lib/*.dll
			)

	MESSAGE("Copy SDL2_image DLLs into ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
	file(COPY ${SDL2_IMAGE_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

# *******************************************
# ************ SDL2TTF LIBRARY **************
# ******************************************
# Set variables before finding SDL2TTF Library
# Helps to find where SDL2TTF libraries are saved
if(WIN32)
	# find the SDL2_ttf-x.y.z folder in the lib directory.
	file(GLOB SDL2TTFDIR "${LIBS_DIR}/SDL2_ttf-[\\.|0-9]*")
	set(ENV{SDL2TTFDIR} ${SDL2TTFDIR})


	# if SDL lib folder exists
	if(SDL2TTFDIR MATCHES "$^")

		if (MSVC)
			set(SDL2_TTF_URL ${SDL2_TTF_VC_URL})
			set(SDL2_TTF_ARCHIVE ${SDL2_TTF_VC_ARCHIVE})
		else ()
			set(SDL2_TTF_URL ${SDL2_TTF_MINGW_URL})
			set(SDL2_TTF_ARCHIVE ${SDL2_TTF_MINGW_ARCHIVE})
		endif ()

		message("Downloading SDL2 library from ${SDL2_TTF_URL}")
		file(DOWNLOAD ${SDL2_TTF_URL} "${LIBS_DIR}/${SDL2_TTF_ARCHIVE}")

		message("Extracting SDL2_TTF library")
		file(ARCHIVE_EXTRACT
				INPUT "${LIBS_DIR}/${SDL2_TTF_ARCHIVE}"
				DESTINATION ${LIBS_DIR})
		file(REMOVE ${LIBS_DIR}/${SDL2_TTF_ARCHIVE})

		file(GLOB SDL2TTFDIR "${LIBS_DIR}/SDL2_ttf-[\\.|0-9]*")
		set(ENV{SDL2TTFDIR} ${SDL2TTFDIR})

		# Moving files
		if (MSVC)
			file(COPY "${SDL2TTFDIR}/lib/x64/" DESTINATION "${SDL2TTFDIR}/lib/")
		else ()
			file(COPY "${SDL2TTFDIR}/x86_64-w64-mingw32/bin/" DESTINATION "${SDL2TTFDIR}/bin/")
			file(COPY "${SDL2TTFDIR}/x86_64-w64-mingw32/lib/" DESTINATION "${SDL2TTFDIR}/lib/")

			file(COPY "${SDL2TTFDIR}/x86_64-w64-mingw32/include/SDL2/" DESTINATION "${SDL2TTFDIR}/include/"
					FILES_MATCHING PATTERN "*.h")
		endif ()

		# Cleaning/Removing unused files
		# MinGW
		file(REMOVE_RECURSE "${SDL2TTFDIR}/i686-w64-mingw32")
		file(REMOVE_RECURSE "${SDL2TTFDIR}/x86_64-w64-mingw32")
		file(REMOVE_RECURSE "${SDL2TTFDIR}/test")

		# MSVC
		file(REMOVE_RECURSE "${SDL2TTFDIR}/lib/x86")
		file(REMOVE_RECURSE "${SDL2TTFDIR}/lib/x64")

	endif ()

endif()


Find_Package (SDL2TTF REQUIRED )

if(NOT SDL2TTF_FOUND)
	MESSAGE(FATAL_ERROR "SDL2TTF not found !")
endif()

if (WIN32)
	file(GLOB
			SDL2TTF_DLL
			${SDL2TTFDIR}/bin/*.dll
			${SDL2TTFDIR}/lib/*.dll
			)

	MESSAGE("Copy SDL2_ttf DLLs into ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
	file(COPY ${SDL2TTF_DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

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
include_directories(${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} "./src/")
add_executable(manual-control ${pendulum_files} "./src/manual/main-manual.cpp")
target_link_libraries(manual-control ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${SDL2TTF_LIBRARY})
target_compile_definitions(manual-control PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")

# Sub project for the training of the TPG
file(GLOB
	training_files
	./src/training/*.cpp
	./src/training/*.h
	params.json
)
include_directories(${GEGELATI_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} "./src/")
add_executable(tpg-training ${pendulum_files} ${training_files})
target_link_libraries(tpg-training ${GEGELATI_LIBRARIES} ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${SDL2TTF_LIBRARY})
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
include_directories(${GEGELATI_INCLUDE_DIRS} ${SDL2_INCLUDE_DIR} ${SDL2IMAGE_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} "./src/" "./src/training/")
add_executable(tpg-inference ${pendulum_files} ${inference_files})
target_link_libraries(tpg-inference ${GEGELATI_LIBRARIES} ${SDL2_LIBRARY} ${SDL2IMAGE_LIBRARY} ${SDL2TTF_LIBRARY})
target_compile_definitions(tpg-inference PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")
