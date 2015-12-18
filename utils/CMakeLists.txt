cmake_minimum_required(VERSION 2.8.4)
#----------------------------------------------------------
# Project
#----------------------------------------------------------
set(PROJECT_NAME utils)
project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g -O2")

include_directories(${PROJECT_TOP_INCLUDE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

FILE(GLOB UTILS_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)
add_library(${PROJECT_NAME} ${UTILS_SOURCE_FILES})

#----------------------------------------------------------
# Query Controller Top Level Test
#----------------------------------------------------------
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(UTILS_TEST_NAME ${PROJECT_NAME}_test.out)

include_directories(${GOOGLETEST_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

add_executable(${UTILS_TEST_NAME} ${TEST_SRC_FILES})
add_test(NAME ${UTILS_TEST_NAME} COMMAND ${UTILS_TEST_NAME})

target_link_libraries(${UTILS_TEST_NAME} ${GTEST_BOTH_LIBRARIES} utils)

