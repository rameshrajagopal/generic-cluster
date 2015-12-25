cmake_minimum_required(VERSION 2.8.4)
find_package(Boost REQUIRED COMPONENTS system filesystem)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "Boost found, include dir: ${Boost_INCLUDE_DIRS}")
    set(Boost_System_STATIC_LIB /usr/lib/x86_64-linux-gnu/libboost_system.a)
    set(Crypto_STATIC_LIB /usr/lib/x86_64-linux-gnu/libcrypto.so)
    set(Ssl_STATIC_LIB /usr/lib/x86_64-linux-gnu/libssl.so)
else()
    message(WARNING "Boost NOT found")
endif()
find_package(Nghttp2 REQUIRED)
if(Nghttp2_FOUND)
    include_directories(${Nghttp2_INCLUDE_DIRS})
    message(STATUS "Boost found, include dir: ${Nghttp2_INCLUDE_DIRS}")
    set(Nghttp2_STATIC_LIB ${Nghttp2_STATIC_LIB_PATH}/libnghttp2.a)
    set(Nghttp2_Asio_STATIC_LIB ${Nghttp2_STATIC_LIB_PATH}/libnghttp2_asio.a)
else()
    message(WARNING "Nghttp2 NOT found")
endif()
#----------------------------------------------------------
# Project
#----------------------------------------------------------
set(PROJECT_NAME request_controller)
project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g -O2")

include_directories(${PROJECT_TOP_INCLUDE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_ROOT_DIR}/utils/include)
include_directories(${PROJECT_ROOT_DIR}/rpc/common/include)

SET(REQUEST_CONTROLLER_INCLUDE_DIRS 
    ${Nghttp2_INCLUDE_DIR}/nghttp2
    )
set(REQUEST_CONTROLLER_LIB_NAME request_controller)

FILE(GLOB REQUEST_CONTROLLER_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)
add_library(${REQUEST_CONTROLLER_LIB_NAME} ${REQUEST_CONTROLLER_SOURCE_FILES})
target_include_directories(${REQUEST_CONTROLLER_LIB_NAME} PUBLIC ${REQUEST_CONTROLLER_INCLUDE_DIRS})
target_link_libraries(${REQUEST_CONTROLLER_LIB_NAME} curl ${Nghttp2_STATIC_LIB} ${Nghttp2_Asio_STATIC_LIB} ${Boost_System_STATIC_LIB} ${Crypto_STATIC_LIB} ${Ssl_STATIC_LIB})

#----------------------------------------------------------
# Query Controller Top Level Test
#----------------------------------------------------------
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(REQUEST_CONTROLLER_TEST_NAME ${PROJECT_NAME}_test.out)

include_directories(${GOOGLETEST_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/test/*.cpp)

add_executable(${REQUEST_CONTROLLER_TEST_NAME} ${TEST_SRC_FILES})
add_test(NAME ${REQUEST_CONTROLLER_TEST_NAME} COMMAND ${REQUEST_CONTROLLER_TEST_NAME})

target_include_directories(${REQUEST_CONTROLLER_TEST_NAME} PUBLIC
    ${GOOGLETEST_DIR}/include ${PROJECT_SOURCE_DIR}/test
    ${REQUEST_CONTROLLER_INCLUDE_DIRS})

target_link_libraries(${REQUEST_CONTROLLER_TEST_NAME} ${GTEST_BOTH_LIBRARIES} request_controller ${Nghttp2_STATIC_LIB} ${Nghttp2_Asio_STATIC_LIB} ${Boost_System_STATIC_LIB} ${Crypto_STATIC_LIB} ${Ssl_STATIC_LIB})

