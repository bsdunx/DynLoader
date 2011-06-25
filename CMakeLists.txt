# ################################################################################
# Project name
#
PROJECT(PDL)

# ################################################################################
# Project version
#
SET(PDL_VERSION_MAJOR 0)
SET(PDL_VERSION_MINOR 3)
SET(PDL_VERSION_PATCH 900)

SET(PDL_VERSION
	"${PDL_VERSION_MAJOR}.${PDL_VERSION_MINOR}.${PDL_VERSION_PATCH}")

# ################################################################################
# CMake version check
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

# ################################################################################
# Check Includes
#
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckLibraryExists)
INCLUDE(CheckCXXSourceCompiles)

# ################################################################################
# Check libraries
#

FIND_PACKAGE(Threads)

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
  CHECK_LIBRARY_EXISTS(dl dlopen "" HAVE_LIBDL)
  IF(NOT HAVE_LIBDL)
    MESSAGE(FATAL_ERROR "Cannot find libdl")
  ENDIF()
ENDIF(CMAKE_SYSTEM_NAME MATCHES "Linux")

# ################################################################################
# Compiler settings
#
# This all needs to be cleaned up.
IF(WIN32)
  IF(MSVC)
    SET(CMAKE_BUILD_ON_VISUAL_STUDIO 1)
  ENDIF()
ENDIF()

SET(CMAKE_BUILD_TYPE Debug)

ENABLE_TESTING()

IF(CMAKE_COMPILER_IS_GNUCXX MATCHES "1")
  SET(CMAKE_CXX_FLAGS "-Wall -pedantic -Weffc++ -fno-rtti -Wno-long-long")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -D_DEBUG -O")
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2 -funroll-loops")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -g -O2")
ELSEIF(CMAKE_BUILD_ON_VISUAL_STUDIO MATCHES "1")
  SET(CMAKE_CXX_FLAGS "/EHsc /GR- /fp:fast /Wall /W4 /wd4668 /wd4820 /wd4548 /wd4710 /wd4571 /wd4127 /wd4100 /wd4512 /wd4706 /wd4242")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /MDd -D_DEBUG /RTC1 /GS /Zi /Od")
  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /MD -DNDEBUG /O2")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} /MD -DNDEBUG /Zi /O2")
  SET(CMAKE_CXX_STANDARD_LIBRARIES "User32.lib")
ENDIF()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/common/include)
INCLUDE_DIRECTORIES(libpdl/include)

ADD_SUBDIRECTORY(libpdl)
ADD_SUBDIRECTORY(foo)

