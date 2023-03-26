cmake_minimum_required(VERSION 3.0)

project(instrumentation)

add_library(instrumentation STATIC
    src/FunctionTimer.cpp
    src/Instrumentor.cpp
)

target_include_directories(instrumentation PUBLIC include)

set(GTEST_ROOT test)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(mylibrary_test test/instrumentation_test.cpp)
target_link_libraries(mylibrary_test instrumentation GTest::GTest GTest::Main)
