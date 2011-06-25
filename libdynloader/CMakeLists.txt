# ################################################################################
# Project version
#
#SET(PDL_VERSION_MAJOR 0)
#SET(PDL_VERSION_MINOR 3)
#SET(PDL_VERSION_PATCH 900)

#SET(PDL_VERSION
#	"${PDL_VERSION_MAJOR}.${PDL_VERSION_MINOR}.${PDL_VERSION_PATCH}")

# ################################################################################
# Library source
#
FILE(GLOB LIBDYNLOADER_SRCS 				src/*.cpp)
FILE(GLOB LIBDYNLOADER_INCS 				include/*.hpp
							include/*.h)

SOURCE_GROUP("LibDynLoader Sources" FILES ${LIBDYNLOADER_SRCS})
SOURCE_GROUP("LibDynLoader Headers" FILES ${LIBDYNLOADER_INCS})

# ################################################################################
# Library binary
#
ADD_LIBRARY(libdynloader SHARED                        ${LIBDYNLOADER_SRCS} ${LIBDYNLOADER_INCS})
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
  TARGET_LINK_LIBRARIES(libdynloader c dl)
ENDIF()

SET_TARGET_PROPERTIES(libdynloader PROPERTIES          OUTPUT_NAME libdynloader)
SET_TARGET_PROPERTIES(libdynloader PROPERTIES          PREFIX "" IMPORT_PREFIX "")
SET_TARGET_PROPERTIES(libdynloader PROPERTIES          LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(libdynloader PROPERTIES          DEFINE_SYMBOL PDL_EXPORT)
SET_TARGET_PROPERTIES(libdynloader PROPERTIES          COMPILE_FLAGS -DPDL_SHARED)
SET_TARGET_PROPERTIES(libdynloader PROPERTIES          VERSION ${LIBDYNLOADER_VERSION}
                                                 SOVERSION ${LIBDYNLOADER_VERSION_MAJOR})

ADD_LIBRARY(libdynloader-static STATIC                 ${LIBDYNLOADER_SRCS} ${LIBDYNLOADER_INCS})
IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
  TARGET_LINK_LIBRARIES(libdynloader-static c dl)
ENDIF()

SET_TARGET_PROPERTIES(libdynloader-static PROPERTIES   OUTPUT_NAME libdynloader-static)
SET_TARGET_PROPERTIES(libdynloader-static PROPERTIES   PREFIX "" IMPORT_PREFIX "")
SET_TARGET_PROPERTIES(libdynloader-static PROPERTIES   LINKER_LANGUAGE CXX )
SET_TARGET_PROPERTIES(libdynloader-static PROPERTIES   VERSION ${LIBDYNLOADER_VERSION}
                                                 SOVERSION ${LIBDYNLOADER_VERSION_MAJOR})

# ################################################################################
# Installation

# Install headers
INSTALL(FILES                                    include/platform.h
                                                 include/DynClass.hpp
                                                 include/DynLoader.hpp
                                                 include/LoaderException.hpp
        DESTINATION                              include/libdynloader)

# Install binaries
INSTALL(TARGETS                                  libdynloader
                                                 libdynloader-static
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
ADD_EXECUTABLE(TestLoaderException               tests/TestLoaderException.cpp ${LIBDYNLOADER_SRCS})

IF(CMAKE_SYSTEM_NAME MATCHES "Linux")
TARGET_LINK_LIBRARIES(TestLoaderException      dl)
ENDIF()

ADD_TEST(TestLoaderException                     ${EXECUTABLE_OUTPUT_PATH}/TestLoaderException)

# Test DynLib class
ADD_EXECUTABLE(TestDynLib                        tests/TestDynLib.cpp ${LIBDYNLOADER_SRCS})
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
ADD_EXECUTABLE(TestDynLibManager                 tests/TestDynLibManager.cpp ${LIBDYNLOADER_SRCS})
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
TARGET_LINK_LIBRARIES(TestDynLoader              libdynloader)

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

