# *******************************************
# ************* CMake Content ***************
# *******************************************
# This CMake setups the following libraries
# 
# - gegelati
# - SDL2
# - SDL2_image
# - SDL2_ttf

set(GEGELATI_VERSION "1.3.1")
set(GEGELATI_MSVC_ARCHIVE "gegelatilib-msvc-${GEGELATI_VERSION}.zip")
set(GEGELATI_MSVC_URL "https://github.com/gegelati/gegelati/releases/download/v${GEGELATI_VERSION}/${GEGELATI_MSVC_ARCHIVE}")

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

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${LIBS_DIR}/cmake_modules/")

# *******************************************
# *********** GEGELATI LIBRARY **************
# *******************************************

if(WIN32)

	# find the Gegelati-x.y.z folder in the lib directory.
	file(GLOB GEGELATI_ROOT_DIR "${LIBS_DIR}/gegelatilib-[\\.|0-9]*")
	set(ENV{GEGELATI_DIR} ${GEGELATI_ROOT_DIR})

	# if Gegelati lib folder doesn't exist
	if(GEGELATI_ROOT_DIR MATCHES "$^")

		if (MSVC)
		  set(GEGELATI_ARCHIVE ${GEGELATI_MSVC_ARCHIVE})
		  set(GEGELATI_URL ${GEGELATI_MSVC_URL})
  
		  message("Downloading GEGELATI library from ${GEGELATI_URL}")
		  file(DOWNLOAD ${GEGELATI_URL} "${LIBS_DIR}/${GEGELATI_ARCHIVE}")
  
		  message("Extracting GEGELATI library")
		  file(ARCHIVE_EXTRACT
		  		INPUT "${LIBS_DIR}/${GEGELATI_ARCHIVE}"
		  		DESTINATION ${LIBS_DIR})
		  file(REMOVE ${LIBS_DIR}/${GEGELATI_ARCHIVE})

		else()

		  message("-- Building GEGELATI")
		  # Setup git clone
		  configure_file("${LIBS_DIR}/CMakeLists.txt.in" ${CMAKE_CURRENT_BINARY_DIR}/gegelati-download/CMakeLists.txt)
		  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
		    RESULT_VARIABLE result
		    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gegelati-download )
		  if(result)
		    message(FATAL_ERROR "CMake step for gegelati failed: ${result}")
		  endif()
		  # Do git clone
		  execute_process(COMMAND ${CMAKE_COMMAND} --build .
		    RESULT_VARIABLE result
		    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gegelati-download )
		  if(result)
		    message(FATAL_ERROR "Build step for gegelati failed: ${result}")
		  endif()
		  # Configure gegelati cmake
		  execute_process(COMMAND ${CMAKE_COMMAND} ../gegelati-src -G "${CMAKE_GENERATOR}" -DSKIP_DOXYGEN_BUILD=ON -DBUILD_TESTING=OFF
		  RESULT_VARIABLE result
		  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gegelati-build )
		  if(result)
		  	message(FATAL_ERROR "Build step for gegelati failed: ${result}")
		  endif()
		  # Build gegelati 
		  execute_process(COMMAND ${CMAKE_COMMAND} --build . --target install -j
		  RESULT_VARIABLE result
		  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gegelati-build )
		  if(result)
		  	message(FATAL_ERROR "Build step for gegelati failed: ${result}")
		  endif()
		  # Copy to lib folder
		  file(GLOB SRC_GEGELATI_ROOT_DIR "${CMAKE_CURRENT_BINARY_DIR}/gegelati-build/gegelatilib-[\\.|0-9]*")
		  get_filename_component(GEGELATI_DIR_NAME "${SRC_GEGELATI_ROOT_DIR}" NAME)
		  file(COPY "${SRC_GEGELATI_ROOT_DIR}/" DESTINATION "${LIBS_DIR}/${GEGELATI_DIR_NAME}")
		  
		endif()

		file(GLOB GEGELATI_ROOT_DIR "${LIBS_DIR}/gegelatilib-[\\.|0-9]*")
		set(ENV{GEGELATI_DIR} ${GEGELATI_ROOT_DIR})
	endif ()

endif()

find_package(GEGELATI REQUIRED)
set(GEGELATI_INCLUDE_DIRS ${GEGELATI_INCLUDE_DIRS})

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