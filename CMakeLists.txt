cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(putong VERSION 0.0.1 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(cmake-modules
  GIT_REPOSITORY https://github.com/abs-tudelft/cmake-modules.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(cmake-modules)
include(CompileUnits)

add_compile_unit(
  NAME putong
  TYPE INTERFACE
)

add_compile_unit(
  NAME putong::tests
  TYPE TESTS
  PRPS
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
  SRCS
    test/putong/test_timer.cpp
  DEPS
    putong
)

compile_units()
