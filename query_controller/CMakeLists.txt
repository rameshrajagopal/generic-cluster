cmake_minimum_required(VERSION 2.8.4)
#----------------------------------------------------------
# Project
#----------------------------------------------------------
set(PROJECT_NAME query_controller)
project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g -O2")

include_directories(${PROJECT_TOP_INCLUDE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

FILE(GLOB QUERY_CONTROLLER_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)
add_library(${PROJECT_NAME} ${QUERY_CONTROLLER_SOURCE_FILES})

#----------------------------------------------------------
# Query Controller Top Level Test
#----------------------------------------------------------
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(QUERY_CONTROLLER_TEST_NAME ${PROJECT_NAME}_test.out)

include_directories(${GOOGLETEST_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

add_executable(${QUERY_CONTROLLER_TEST_NAME} ${TEST_SRC_FILES})
add_test(NAME ${QUERY_CONTROLLER_TEST_NAME} COMMAND ${QUERY_CONTROLLER_TEST_NAME})

target_link_libraries(${QUERY_CONTROLLER_TEST_NAME} ${GTEST_BOTH_LIBRARIES} query_controller)

