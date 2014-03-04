file(GLOB LIBDYNLOADER_SRCS src/*.cpp)
file(GLOB LIBDYNLOADER_INCS include/*.hpp include/*.h ${CMAKE_CURRENT_BINARY_DIR}/include/*.hpp)

source_group("LibDynLoader Sources" FILES ${LIBDYNLOADER_SRCS})
source_group("LibDynLoader Headers" FILES ${LIBDYNLOADER_INCS})

#include_directories(include)

add_library(libdynloader SHARED ${LIBDYNLOADER_SRCS} ${LIBDYNLOADER_INCS})

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(libdynloader c dl)
endif()

set_target_properties(libdynloader PROPERTIES PREFIX "")
set_target_properties(libdynloader PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(libdynloader PROPERTIES DEFINE_SYMBOL EXPORTS)
set_target_properties(libdynloader PROPERTIES COMPILE_FLAGS -DSHARED)
set_target_properties(libdynloader PROPERTIES VERSION ${DynLoader_VERSION}
                                              SOVERSION ${DynLoader_VERSION_MAJOR})

add_library(libdynloader-static STATIC ${LIBDYNLOADER_SRCS} ${LIBDYNLOADER_INCS})
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(libdynloader-static c dl)
endif()

set_target_properties(libdynloader-static PROPERTIES PREFIX "")
set_target_properties(libdynloader-static PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(libdynloader-static PROPERTIES VERSION ${DynLoader_VERSION})

install(FILES 
    include/platform.h include/DynClass.hpp include/DynLoader.hpp include/LoaderException.hpp
    DESTINATION include/libdynloader)

install(TARGETS libdynloader libdynloader-static DESTINATION lib)

add_library(libtest_module MODULE tests/TestClass.cpp tests/TestClass.hpp tests/TestInterface.hpp include/platform.h)
add_dependencies(libtest_module libdynloader)
set_target_properties(libtest_module PROPERTIES PREFIX "")
set_target_properties(libtest_module PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(libtest_module PROPERTIES DEFINE_SYMBOL EXPORTS)
set_target_properties(libtest_module PROPERTIES COMPILE_FLAGS -DSHARED)

add_executable(TestLoaderException tests/TestLoaderException.cpp src/LoaderException.cpp)
add_dependencies(TestLoaderException libdynloader)
set_target_properties(TestLoaderException PROPERTIES PREFIX "")

add_test(TestLoaderException ${OUTPUT_PATH}/TestLoaderException)


#add_executable(TestDynLib tests/TestDynLib.cpp src/DynLib.cpp src/LoaderException.cpp include/DynLib.hpp include/LoaderException.hpp include/DynClass.hpp)
#add_dependencies(TestDynLib libdynloader)
#set_target_properties(TestDynLib PROPERTIES PREFIX "")

#if(CMAKE_SYSTEM_NAME MATCHES "Linux")
#  target_link_libraries(TestDynLib dl)
#endif()

#if(WIN32 AND NOT CYGWIN)
#  add_test(TestDynLib ${OUTPUT_PATH}/TestDynLib libtest_module.dll)
#else()
#  add_test(TestDynLib ${OUTPUT_PATH}/TestDynLib ./libtest_module.so)
#endif()

#add_executable(TestDynLibManager tests/TestDynLibManager.cpp src/DynLibManager.cpp src/DynLib.cpp src/LoaderException.cpp)
#add_dependencies(TestDynLibManager libdynloader)
#set_target_properties(TestDynLibManager PROPERTIES PREFIX "")

#if(CMAKE_SYSTEM_NAME MATCHES "Linux")
#  target_link_libraries(TestDynLibManager dl)
#endif()

#if(WIN32 AND NOT CYGWIN)
#  add_test(TestDynLibManager ${OUTPUT_PATH}/TestDynLibManager libtest_module.dll)
#else()
#  add_test(TestDynLibManager ${OUTPUT_PATH}/TestDynLibManager ./libtest_module.so)
#endif()

add_executable(TestDynLoader tests/TestDynLoader.cpp)
add_dependencies(TestDynLoader libdynloader)
set_target_properties(TestDynLoader PROPERTIES PREFIX "")
target_link_libraries(TestDynLoader libdynloader)

if(WIN32 AND NOT CYGWIN)
  add_test(TestDynLoader ${OUTPUT_PATH}/TestDynLoader libtest_module.dll Test1 Test2)
else()
  add_test(TestDynLoader ${OUTPUT_PATH}/TestDynLoader ./libtest_module.so Test1 Test2)
endif()

