cmake_minimum_required(VERSION 3.0)

project(instrumentation)
file(GLOB_RECURSE CPP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

execute_process(COMMAND cat /proc/cpuinfo OUTPUT_VARIABLE CPUINFO)

# string(REGEX MATCH "cpu MHz[\\s]*:[\\s]*([0-9]+\\.[0-9]+)" CPUFREQ ${CPUINFO})
# string(REGEX MATCH "cpu MHz[ \t]*:[ \t]*([0-9]+\\.[0-9]+)" CPUFREQ ${CPUINFO})
# set(CPU_FREQUENCY ${CMAKE_MATCH_1})
# message("CPUFREQ=${CPUFREQ}")

# set(CPU_FREQUENCY ${CPUFREQ})
# add_compile_definitions(CPU_FREQUENCY=${CPU_FREQUENCY})
# message("CPU_FREQUENCY=${CPU_FREQUENCY}")
add_library(instrumentation STATIC ${CPP_SOURCES})

# add_library(instrumentation STATIC
# src/FunctionTimer.cpp
# src/Instrumentor.cpp
# src/timer.cpp
# src/cachemisscounter.cpp
# )
target_include_directories(instrumentation PUBLIC include)

set_target_properties(instrumentation PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/build/libinstrumentation.a
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()

find_package(GTest 1.11 QUIET MODULE REQUIRED)

if(DEFINED GTest_VERSION)
    set(gtestlib GTest::gtest)
else()
    find_library(gtestlib gtest)

    if(${gtestlib} STREQUAL gtestlib-NOTFOUND)
        message(FATAL_ERROR "Can not find GTest")
    endif()
endif()

message(STATUS "Using GTest library: ${gtestlib}")

# Discover and run all tests in the directory
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test/*_test.cpp")
add_executable(mylibrary_test ${TEST_SOURCES})
target_link_libraries(mylibrary_test instrumentation Threads::Threads ${gtestlib})
target_include_directories(mylibrary_test PRIVATE include)

add_test(NAME mylibrary_test COMMAND mylibrary_test)