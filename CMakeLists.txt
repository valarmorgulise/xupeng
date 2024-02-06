cmake_minimum_required()
project(xupeng)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -0O -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

set(LIB_SRC
  xupeng/log.cc)

add_library(xupeng SHARED ${LIB_SRC})

add_executable(tests/test.cc)
add_dependencies(test xupeng)
target_link_libraries(test xupeng)

SET(EXECUTABLE_OUTPUT_PATH ${PEOJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
