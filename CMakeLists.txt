cmake_minimum_required()
project(xupeng)

include (./cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined")

include_directories(/usr/locl/include)
link_directories(/usr/local/lib)

find_library(YAMLCPP libyaml-cpp.a)

set(LIB_SRC
  xupeng/log.cc
  xupeng/mutex.cc 
  xupeng/thread.cc 
  xupeng/config.cc
  xupeng/util.cc)


set(LIB_LIB 
    xupeng
    pthread
    -L/usr/local/lib -lyaml-cpp
    )

add_library(xupeng SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(xupeng) #__FILE__


add_executable(test tests/test.cc)
add_dependencies(test xupeng) 
target_link_libraries(test ${LIB_LIB})
force_redefine_file_macro_for_sources(test) #__FILE__

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config xupeng) 
target_link_libraries(test_config ${LIB_LIB})
force_redefine_file_macro_for_sources(test_config) #__FILE__

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread xupeng) 
target_link_libraries(test_thread ${LIB_LIB})
force_redefine_file_macro_for_sources(test_thread) #__FILE__

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
