# ################################################################################
# Project version
#
SET(PDL_VERSION_MAJOR 0)
SET(PDL_VERSION_MINOR 3)
SET(PDL_VERSION_PATCH 900)

SET(PDL_VERSION
	"${PDL_VERSION_MAJOR}.${PDL_VERSION_MINOR}.${PDL_VERSION_PATCH}")

# ################################################################################
# Library source
#
FILE(GLOB PDL_SRCS                               src/*.cpp)
FILE(GLOB PDL_INCS                               include/*.hpp
                                                 include/*.h)

SOURCE_GROUP("PDL Sources" FILES ${PDL_SRCS})
SOURCE_GROUP("PDL Headers" FILES ${PDL_INCS})

# ################################################################################
# Library binary
#
ADD_LIBRARY(libpdl SHARED                        ${PDL_SRCS} ${PDL_INCS})
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
  TARGET_LINK_LIBRARIES(libpdl c dl)
ENDIF()

SET_TARGET_PROPERTIES(libpdl PROPERTIES          OUTPUT_NAME libpdl)
SET_TARGET_PROPERTIES(libpdl PROPERTIES          PREFIX "" IMPORT_PREFIX "")
SET_TARGET_PROPERTIES(libpdl PROPERTIES          LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(libpdl PROPERTIES          DEFINE_SYMBOL PDL_EXPORT)
SET_TARGET_PROPERTIES(libpdl PROPERTIES          COMPILE_FLAGS -DPDL_SHARED)
SET_TARGET_PROPERTIES(libpdl PROPERTIES          VERSION ${PDL_VERSION}
                                                 SOVERSION ${PDL_VERSION_MAJOR})

ADD_LIBRARY(libpdl-static STATIC                 ${PDL_SRCS} ${PDL_INCS})
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
  TARGET_LINK_LIBRARIES(libpdl-static c dl)
ENDIF()

SET_TARGET_PROPERTIES(libpdl-static PROPERTIES   OUTPUT_NAME libpdl-static)
SET_TARGET_PROPERTIES(libpdl-static PROPERTIES   PREFIX "" IMPORT_PREFIX "")
SET_TARGET_PROPERTIES(libpdl-static PROPERTIES   LINKER_LANGUAGE CXX )
SET_TARGET_PROPERTIES(libpdl-static PROPERTIES   VERSION ${PDL_VERSION}
                                                 SOVERSION ${PDL_VERSION_MAJOR})

# ################################################################################
# Installation

# Install headers
INSTALL(FILES                                    include/platform.h
                                                 include/DynClass.hpp
                                                 include/DynLoader.hpp
                                                 include/LoaderException.hpp
        DESTINATION                              include/pdl)

# Install binaries
INSTALL(TARGETS                                  libpdl
                                                 libpdl-static
        DESTINATION                              lib)

# ################################################################################
# Test suite
#

# Test libraries
ADD_LIBRARY(libtest_module1 MODULE               tests/TestClass1.cpp
                                                 tests/TestClass1.hpp
                                                 tests/TestInterface.hpp)

SET_TARGET_PROPERTIES(libtest_module1 PROPERTIES OUTPUT_NAME libtest_module1)
SET_TARGET_PROPERTIES(libtest_module1 PROPERTIES PREFIX "" IMPORT_PREFIX "")

ADD_LIBRARY(libtest_module2 MODULE               tests/TestClass2.cpp
                                                 tests/TestClass2.hpp
						 tests/TestInterface.hpp)

SET_TARGET_PROPERTIES(libtest_module2 PROPERTIES OUTPUT_NAME libtest_module2)
SET_TARGET_PROPERTIES(libtest_module2 PROPERTIES PREFIX "" IMPORT_PREFIX "")

# Test LoaderException class
ADD_EXECUTABLE(TestLoaderException               tests/TestLoaderException.cpp ${PDL_SRCS})

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
TARGET_LINK_LIBRARIES(TestLoaderException      dl)
ENDIF()

ADD_TEST(TestLoaderException                     ${EXECUTABLE_OUTPUT_PATH}/TestLoaderException)

# Test DynLib class
ADD_EXECUTABLE(TestDynLib                        tests/TestDynLib.cpp ${PDL_SRCS})
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
TARGET_LINK_LIBRARIES(TestDynLib               dl)
ENDIF()

# DynLib Tests
IF(WIN32 AND NOT CYGWIN)
  ADD_TEST(TestDynLib                            ${EXECUTABLE_OUTPUT_PATH}/TestDynLib libtest_module1.dll)
ELSE()
  ADD_TEST(TestDynLib                            ${EXECUTABLE_OUTPUT_PATH}/TestDynLib ./libtest_module1.so)
ENDIF()

# Test DynamicLibraryManager class
ADD_EXECUTABLE(TestDynLibManager                 tests/TestDynLibManager.cpp ${PDL_SRCS})
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
TARGET_LINK_LIBRARIES(TestDynLibManager        dl)
ENDIF()

# DynamicLibraryManager Tests
IF(WIN32 AND NOT CYGWIN)
  ADD_TEST(TestDynLibManager
    ${EXECUTABLE_OUTPUT_PATH}/TestDynLibManager libtest_module1.dll libtest_module2.dll)
ELSE()
  ADD_TEST(TestDynLibManager                       
    ${EXECUTABLE_OUTPUT_PATH}/TestDynLibManager ./libtest_module1.so ./libtest_module2.so)
ENDIF()

# Test DynLoader class
ADD_EXECUTABLE(TestDynLoader                     tests/TestDynLoader.cpp)
TARGET_LINK_LIBRARIES(TestDynLoader              libpdl)

# DynLoader Tests
IF(WIN32 AND NOT CYGWIN)
  ADD_TEST(TestDynLoader1                          
    ${EXECUTABLE_OUTPUT_PATH}/TestDynLoader libtest_module1.dll Test1 Test2)
  ADD_TEST(TestDynLoader2                        
    ${EXECUTABLE_OUTPUT_PATH}/TestDynLoader libtest_module2.dll Test3 Test4)
ELSE()
  ADD_TEST(TestDynLoader1                          
    ${EXECUTABLE_OUTPUT_PATH}/TestDynLoader ./libtest_module1.so Test1 Test2)
  ADD_TEST(TestDynLoader2                          
    ${EXECUTABLE_OUTPUT_PATH}/TestDynLoader ./libtest_module2.so Test3 Test4)
ENDIF()

