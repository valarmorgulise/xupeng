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
  xupeng/fd_manager.cc
  xupeng/fiber.cc
  xupeng/hook.cc
  xupeng/address.cc
  xupeng/scheduler.cc
  xupeng/bytearray.cc
  xupeng/iomanager.cc
  xupeng/timer.cc
  xupeng/config.cc
  xupeng/socket.cc
  xupeng/util.cc)


set(LIB_LIB 
    xupeng
    pthread
    -L/usr/local/lib -lyaml-cpp
    dl
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

add_executable(test_util tests/test_util.cc)
add_dependencies(test_util xupeng) 
target_link_libraries(test_util ${LIB_LIB})
force_redefine_file_macro_for_sources(test_util) #__FILE__

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber xupeng) 
target_link_libraries(test_fiber ${LIB_LIB})
force_redefine_file_macro_for_sources(test_fiber) #__FILE__

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler xupeng) 
target_link_libraries(test_scheduler ${LIB_LIB})
force_redefine_file_macro_for_sources(test_scheduler) #__FILE__

add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager xupeng) 
target_link_libraries(test_iomanager ${LIB_LIB})
force_redefine_file_macro_for_sources(test_iomanager) #__FILE__

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook xupeng) 
target_link_libraries(test_hook ${LIB_LIB})
force_redefine_file_macro_for_sources(test_hook) #__FILE__

add_executable(test_address tests/test_address.cc)
add_dependencies(test_address xupeng) 
target_link_libraries(test_address ${LIB_LIB})
force_redefine_file_macro_for_sources(test_address) #__FILE__

add_executable(test_socket tests/test_socket.cc)
add_dependencies(test_socket xupeng) 
target_link_libraries(test_socket ${LIB_LIB})
force_redefine_file_macro_for_sources(test_socket) #__FILE__

add_executable(test_bytearray tests/test_bytearray.cc)
add_dependencies(test_bytearray xupeng) 
target_link_libraries(test_bytearray ${LIB_LIB})
force_redefine_file_macro_for_sources(test_bytearray) #__FILE__

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
