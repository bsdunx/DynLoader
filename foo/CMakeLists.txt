project(FOO)

set(FOO_VERSION_MAJOR 0)
set(FOO_VERSION_MINOR 1)
set(FOO_VERSION_PATCH 0)

set(FOO_VERSION
	"${FOO_VERSION_MAJOR}.${FOO_VERSION_MINOR}.${FOO_VERSION_PATCH}")

add_library(libfoo_module MODULE FooClass1.cpp FooClass1.hpp FooInterface.hpp)

add_executable(TestFooMain TestFooMain.cpp TestFoo.cpp)
add_dependencies(TestFooMain libdynloader)
target_link_libraries(TestFooMain libdynloader)

if(WIN32)
  add_library(TestFooLib SHARED TestFooLib.cpp TestFoo.cpp)
  add_dependencies(TestFooLib libdynloader)
  target_link_libraries(TestFooLib libdynloader)
endif(WIN32)
