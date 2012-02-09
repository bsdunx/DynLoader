file(GLOB LIBDYNLOADER_SRCS src/*.cpp)
file(GLOB LIBDYNLOADER_INCS include/*.hpp include/*.h)

source_group("LibDynLoader Sources" FILES ${LIBDYNLOADER_SRCS})
source_group("LibDynLoader Headers" FILES ${LIBDYNLOADER_INCS})

add_library(libdynloader SHARED ${LIBDYNLOADER_SRCS} ${LIBDYNLOADER_INCS})
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(libdynloader c dl)
endif()

set_target_properties(libdynloader PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(libdynloader PROPERTIES DEFINE_SYMBOL PDL_EXPORT)
set_target_properties(libdynloader PROPERTIES COMPILE_FLAGS -DPDL_SHARED)
set_target_properties(libdynloader PROPERTIES VERSION ${LIBDYNLOADER_VERSION}
                                              SOVERSION ${LIBDYNLOADER_VERSION_MAJOR})

add_library(libdynloader-static STATIC ${LIBDYNLOADER_SRCS} ${LIBDYNLOADER_INCS})
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(libdynloader-static c dl)
endif()

set_target_properties(libdynloader-static PROPERTIES LINKER_LANGUAGE CXX )
set_target_properties(libdynloader-static PROPERTIES VERSION ${LIBDYNLOADER_VERSION}
                                                 SOVERSION ${LIBDYNLOADER_VERSION_MAJOR})

install(FILES 
    include/platform.h include/DynClass.hpp include/DynLoader.hpp include/LoaderException.hpp
    DESTINATION
	include/libdynloader)

install(TARGETS libdynloader libdynloader-static DESTINATION lib)

add_library(libtest_module1 MODULE tests/TestClass1.cpp tests/TestClass1.hpp tests/TestInterface.hpp)
add_dependencies(libtest_module1 libdynloader)


add_library(libtest_module2 MODULE tests/TestClass2.cpp tests/TestClass2.hpp tests/TestInterface.hpp)
add_dependencies(libtest_module2 libdynloader)


add_executable(TestLoaderException tests/TestLoaderException.cpp ${LIBDYNLOADER_SRCS})
add_dependencies(TestLoaderException libdynloader)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(TestLoaderException dl)
endif()

add_test(TestLoaderException ${EXECUTABLE_OUTPUT_PATH}/TestLoaderException)


add_executable(TestDynLib tests/TestDynLib.cpp ${LIBDYNLOADER_SRCS})
add_dependencies(TestDynLib libdynloader)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(TestDynLib dl)
endif()

if(WIN32 AND NOT CYGWIN)
  add_test(TestDynLib ${EXECUTABLE_OUTPUT_PATH}/TestDynLib libtest_module1.dll)
else()
  add_test(TestDynLib ${EXECUTABLE_OUTPUT_PATH}/TestDynLib ./libtest_module1.so)
endif()

add_executable(TestDynLibManager tests/TestDynLibManager.cpp ${LIBDYNLOADER_SRCS})
add_dependencies(TestDynLibManager libdynloader)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(TestDynLibManager dl)
endif()

if(WIN32 AND NOT CYGWIN)
  add_test(TestDynLibManager
    ${EXECUTABLE_OUTPUT_PATH}/TestDynLibManager libtest_module1.dll libtest_module2.dll)
else()
  add_test(TestDynLibManager                       
    ${EXECUTABLE_OUTPUT_PATH}/TestDynLibManager ./libtest_module1.so ./libtest_module2.so)
endif()

add_executable(TestDynLoader tests/TestDynLoader.cpp)
add_dependencies(TestLoaderException libdynloader)
target_link_libraries(TestDynLoader libdynloader)

if(WIN32 AND NOT CYGWIN)
  add_test(TestDynLoader1                          
    ${EXECUTABLE_OUTPUT_PATH}/TestDynLoader libtest_module1.dll Test1 Test2)
  add_test(TestDynLoader2                        
    ${EXECUTABLE_OUTPUT_PATH}/TestDynLoader libtest_module2.dll Test3 Test4)
else()
  add_test(TestDynLoader1                          
    ${EXECUTABLE_OUTPUT_PATH}/TestDynLoader ./libtest_module1.so Test1 Test2)
  add_test(TestDynLoader2                          
    ${EXECUTABLE_OUTPUT_PATH}/TestDynLoader ./libtest_module2.so Test3 Test4)
endif()
