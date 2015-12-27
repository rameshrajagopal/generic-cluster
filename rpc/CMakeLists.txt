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
# Common
#----------------------------------------------------------
set(PROJECT_NAME rpc_common)
project(${PROJECT_NAME})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -g -O2")

include_directories(${PROJECT_ROOT_DIR}/include)
include_directories(${PROJECT_ROOT_DIR}/utils/include)
include_directories(${PROJECT_SOURCE_DIR}/common/include)

SET(COMMON_INCLUDE_DIRS 
    ${Nghttp2_INCLUDE_DIR}/nghttp2
    )

set(RPC_COMMON_LIB_NAME rpc_common)
FILE(GLOB RPC_COMMON_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/common/src/*.cpp
)

add_library(${RPC_COMMON_LIB_NAME} ${RPC_COMMON_SOURCE_FILES})

target_include_directories(${RPC_COMMON_LIB_NAME} PUBLIC ${COMMON_INCLUDE_DIRS})
target_link_libraries(${RPC_COMMON_LIB_NAME} curl ${Nghttp2_STATIC_LIB} ${Nghttp2_Asio_STATIC_LIB} ${Boost_System_STATIC_LIB} ${Crypto_STATIC_LIB} ${Ssl_STATIC_LIB})

#----------------------------------------------------------
# Common top level test
#----------------------------------------------------------
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(RPC_TEST_NAME ${PROJECT_NAME}_test.out)

include_directories(${GOOGLETEST_DIR}/include)
include_directories(${PROJECT_ROOT_DIR}/utils/include)
include_directories(${PROJECT_SOURCE_DIR}/common/include)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/common/test/*.cpp)

add_executable(${RPC_TEST_NAME} ${TEST_SRC_FILES})
add_test(NAME ${RPC_TEST_NAME} COMMAND ${RPC_TEST_NAME})

target_include_directories(${RPC_TEST_NAME} PUBLIC
    ${GOOGLETEST_DIR}/include
    ${PROJECT_SOURCE_DIR}/test
    ${COMMON_INCLUDE_DIRS}
)

target_link_libraries(${RPC_TEST_NAME} ${GTEST_BOTH_LIBRARIES} rpc_common ${Nghttp2_STATIC_LIB} ${Nghttp2_Asio_STATIC_LIB} ${Boost_System_STATIC_LIB} ${Crypto_STATIC_LIB} ${Ssl_STATIC_LIB})

#----------------------------------------------------------
# Client
#----------------------------------------------------------
set(PROJECT_NAME rpc_client)
project(${PROJECT_NAME})

include_directories(${PROJECT_ROOT_DIR}/include)
include_directories(${PROJECT_ROOT_DIR}/utils/include)
include_directories(${PROJECT_SOURCE_DIR}/client/include)

set(RPC_CLIENT_LIB_NAME rpc_client)
FILE(GLOB RPC_CLIENT_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/client/src/*.cpp
)

add_library(${RPC_CLIENT_LIB_NAME} ${RPC_CLIENT_SOURCE_FILES})
SET(CLUSTER_CLIENT_LIB_NAME ${PROJECT_SOURCE_DIR}/client/include)

target_include_directories(${RPC_CLIENT_LIB_NAME} PUBLIC ${COMMON_INCLUDE_DIRS})
target_link_libraries(${RPC_CLIENT_LIB_NAME} curl ${Nghttp2_STATIC_LIB} ${Nghttp2_Asio_STATIC_LIB} ${Boost_System_STATIC_LIB} ${Crypto_STATIC_LIB} ${Ssl_STATIC_LIB})

#----------------------------------------------------------
# Common top level test
#----------------------------------------------------------
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(RPC_TEST_NAME ${PROJECT_NAME}_test.out)

include_directories(${GOOGLETEST_DIR}/include)
include_directories(${PROJECT_ROOT_DIR}/utils/include)
include_directories(${PROJECT_SOURCE_DIR}/client/include)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/client/test/*.cpp)

add_executable(${RPC_TEST_NAME} ${TEST_SRC_FILES})
add_test(NAME ${RPC_TEST_NAME} COMMAND ${RPC_TEST_NAME})

target_include_directories(${RPC_TEST_NAME} PUBLIC
    ${GOOGLETEST_DIR}/include
    ${PROJECT_SOURCE_DIR}/test
    ${CLIENT_INCLUDE_DIRS}
)

target_link_libraries(${RPC_TEST_NAME} ${GTEST_BOTH_LIBRARIES} rpc_client rpc_common ${Nghttp2_STATIC_LIB} ${Nghttp2_Asio_STATIC_LIB} ${Boost_System_STATIC_LIB} ${Crypto_STATIC_LIB} ${Ssl_STATIC_LIB})

#----------------------------------------------------------
# Server
#----------------------------------------------------------
set(PROJECT_NAME rpc_server)
project(${PROJECT_NAME})

include_directories(${PROJECT_ROOT_DIR}/include)
include_directories(${PROJECT_ROOT_DIR}/utils/include)
include_directories(${PROJECT_SOURCE_DIR}/server/include)

set(RPC_SERVER_LIB_NAME rpc_server)
FILE(GLOB RPC_SERVER_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/server/src/*.cpp
)

add_library(${RPC_SERVER_LIB_NAME} ${RPC_SERVER_SOURCE_FILES})
SET(CLUSTER_SERVER_LIB_NAME ${PROJECT_SOURCE_DIR}/server/include)

target_include_directories(${RPC_SERVER_LIB_NAME} PUBLIC ${COMMON_INCLUDE_DIRS})
target_link_libraries(${RPC_SERVER_LIB_NAME} curl ${Nghttp2_STATIC_LIB} ${Nghttp2_Asio_STATIC_LIB} ${Boost_System_STATIC_LIB} ${Crypto_STATIC_LIB} ${Ssl_STATIC_LIB})

#----------------------------------------------------------
# Common top level test
#----------------------------------------------------------
enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(RPC_TEST_NAME ${PROJECT_NAME}_test.out)

include_directories(${GOOGLETEST_DIR}/include)
include_directories(${PROJECT_ROOT_DIR}/utils/include)
include_directories(${PROJECT_SOURCE_DIR}/server/include)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/server/test/*.cpp)

add_executable(${RPC_TEST_NAME} ${TEST_SRC_FILES})
add_test(NAME ${RPC_TEST_NAME} COMMAND ${RPC_TEST_NAME})

target_include_directories(${RPC_TEST_NAME} PUBLIC
    ${GOOGLETEST_DIR}/include
    ${PROJECT_SOURCE_DIR}/test
    ${SERVER_INCLUDE_DIRS}
)

target_link_libraries(${RPC_TEST_NAME} ${GTEST_BOTH_LIBRARIES} rpc_server rpc_common ${Nghttp2_STATIC_LIB} ${Nghttp2_Asio_STATIC_LIB} ${Boost_System_STATIC_LIB} ${Crypto_STATIC_LIB} ${Ssl_STATIC_LIB})
