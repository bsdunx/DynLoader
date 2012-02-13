cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0002 NEW)
  cmake_policy(SET CMP0003 NEW)
  cmake_policy(SET CMP0004 NEW)
  cmake_policy(SET CMP0005 NEW)
  cmake_policy(SET CMP0008 NEW)
  cmake_policy(SET CMP0010 NEW)
  cmake_policy(SET CMP0012 NEW)
  cmake_policy(SET CMP0013 NEW)
  cmake_policy(SET CMP0014 NEW)
  cmake_policy(SET CMP0015 NEW)
  cmake_policy(SET CMP0016 NEW)
endif()

include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckCXXSourceCompiles)

project(DynLoader)

set(DynLoader_VERSION_MAJOR 0)
set(DynLoader_VERSION_MINOR 1)
set(DynLoader_VERSION_PATCH 0)

set(DynLoader_VERSION
	"${DynLoader_VERSION_MAJOR}.${DynLoader_VERSION_MINOR}.${DynLoader_VERSION_PATCH}")

if(WIN32)
  if(MSVC)
    set(CMAKE_BUILD_ON_VISUAL_STUDIO 1)
  endif()
endif()

if(CMAKE_COMPILER_IS_GNUCXX MATCHES "1")
  set(CMAKE_CXX_FLAGS "-std=gnu++0x -Wall -pedantic -Weffc++ -fno-rtti")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -D_DEBUG -O")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O2")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} -g -O2")
elseif(CMAKE_BUILD_ON_VISUAL_STUDIO MATCHES "1")
  set(CMAKE_CXX_FLAGS "/EHsc /GR- /Wall /WL /W4 /wd4251 /wd4668 /wd4820 /wd4548 /wd4710 /wd4571 /wd4127 /wd4100 /wd4512 /wd4706 /wd4242 /DWIN32_LEAN_AND_MEAN")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /MDd -D_DEBUG /RTC1 /GS /Zi /Od")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /MD -DNDEBUG /O2")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS} /MD -DNDEBUG /Zi /O2")
  set(CMAKE_CXX_STANDARD_LIBRARIES "User32.lib")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  check_library_exists(dl dlopen "" HAVE_LIBDL)
  if(NOT HAVE_LIBDL)
    message(FATAL_ERROR "Cannot find libdl")
  endif()
endif(CMAKE_SYSTEM_NAME MATCHES "Linux")

set(CMAKE_BUILD_TYPE Debug)

enable_testing()

include_directories(libdynloader/include)

add_subdirectory(libdynloader)

