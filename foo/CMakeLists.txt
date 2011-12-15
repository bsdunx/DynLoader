# ################################################################################
# Project name
#
PROJECT(FOO)

# ################################################################################
# Project version
#
SET(FOO_VERSION_MAJOR 0)
SET(FOO_VERSION_MINOR 1)
SET(FOO_VERSION_PATCH 0)

SET(FOO_VERSION
	"${FOO_VERSION_MAJOR}.${FOO_VERSION_MINOR}.${FOO_VERSION_PATCH}")

# ################################################################################
# Foo test apps
#

# Test Foo libraries
ADD_LIBRARY(libfoo_module MODULE                 FooClass1.cpp
                                                 FooClass1.hpp
                                                 FooInterface.hpp)

SET_TARGET_PROPERTIES(libfoo_module PROPERTIES   OUTPUT_NAME libfoo_module)
SET_TARGET_PROPERTIES(libfoo_module PROPERTIES   PREFIX "" IMPORT_PREFIX "")

# TestFooMain
ADD_EXECUTABLE(TestFooMain                       TestFooMain.cpp 
                                                 TestFoo.cpp)
ADD_DEPENDENCIES(TestFooMain 					 libdynloader)
TARGET_LINK_LIBRARIES(TestFooMain                libdynloader
	${CMAKE_THREAD_LIBS_INIT})

# TestFooWin
IF(WIN32 AND NOT CYGWIN)
  ADD_EXECUTABLE(TestFooWin WIN32                TestFooWin.cpp
                                                 TestFoo.cpp)

  ADD_DEPENDENCIES(TestFooWin 					 libdynloader)
  TARGET_LINK_LIBRARIES(TestFooWin               libdynloader Gdi32)
ENDIF(WIN32 AND NOT CYGWIN)

# TestFooLib
IF(WIN32)
  ADD_LIBRARY(TestFooLib SHARED			 		 TestFooLib.cpp
                                                 TestFoo.cpp)
  ADD_DEPENDENCIES(TestFooLib 					 libdynloader)
  TARGET_LINK_LIBRARIES(TestFooLib               libdynloader)
ENDIF(WIN32)

