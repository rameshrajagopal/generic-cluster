cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g -O2")

find_package(Boost REQUIRED COMPONENTS system filesystem)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Boost found, include dir: ${Boost_INCLUDE_DIRS}")
else()
    message(WARNING "Boost NOT found")
endif()

#----------------------------------------------------------
# Project
#----------------------------------------------------------
set(PROJECT_NAME cluster)
project(${PROJECT_NAME})

#set(SHERLOCK_MODULE SherlockCluster)
#set(SHERLOCK_MASTER_MODULE SherlockMaster)
#set(SHERLOCK_SLAVE_MODULE SherlockSlave)

#set(MARINA_MASTER_MODULE MarinaMaster)
#set(MARINA_SLAVE_MODULE MarinaSlave)

#SET(COMMON_INCLUDE_DIRS
#    ${Folly_INCLUDE_DIR}/folly
#    ${PROJECT_SOURCE_DIR}/sherlock/include
#)

#----------------------------------------------------------
# Build Cluster common
#----------------------------------------------------------

set(CLUSTER_COMMON_LIB_NAME cluster_common)

FILE(GLOB CLUSTER_COMMON_SOURCE_FILES ${PROJECT_SOURCE_DIR}/common/src/*.cpp)

add_library(${CLUSTER_COMMON_LIB_NAME} ${CLUSTER_COMMON_SOURCE_FILES})

SET(CLUSTER_COMMON_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/common/include)

target_include_directories(${CLUSTER_COMMON_LIB_NAME} PUBLIC ${COMMON_INCLUDE_DIRS} ${CLUSTER_COMMON_INCLUDE_DIRS})
target_link_libraries(${CLUSTER_COMMON_LIB_NAME} 

#----------------------------------------------------------
# Cluster Test
#----------------------------------------------------------
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CLUSTER_COMMON_TEST_NAME cluster_common_test.out)

FILE(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/common/test/*.cpp)

add_executable(${CLUSTER_COMMON_TEST_NAME} ${TEST_SRC_FILES})

add_test(NAME ${CLUSTER_COMMON_TEST_NAME} COMMAND ${CLUSTER_COMMON_TEST_NAME})

target_include_directories(${CLUSTER_COMMON_TEST_NAME} PUBLIC ${COMMON_INCLUDE_DIRS})

target_link_libraries(${CLUSTER_COMMON_TEST_NAME} cluster_common common ${GTEST_BOTH_LIBRARIES})
