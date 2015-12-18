cmake_minimum_required(VERSION 2.8.4)

# where to find cmake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

#----------------------------------------------------------
# Project
#----------------------------------------------------------
set(PROJECT_NAME cluster)
project(${PROJECT_NAME})

# Ensures that we do an out of source build

MACRO(MACRO_ENSURE_OUT_OF_SOURCE_BUILD MSG)
     STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}"
     "${CMAKE_BINARY_DIR}" insource)
     GET_FILENAME_COMPONENT(PARENTDIR ${CMAKE_SOURCE_DIR} PATH)
     STRING(COMPARE EQUAL "${CMAKE_SOURCE_DIR}"
     "${PARENTDIR}" insourcesubdir)
    IF(insource OR insourcesubdir)
        MESSAGE(FATAL_ERROR "${MSG}")
    ENDIF(insource OR insourcesubdir)
ENDMACRO(MACRO_ENSURE_OUT_OF_SOURCE_BUILD)

MACRO_ENSURE_OUT_OF_SOURCE_BUILD("${CMAKE_PROJECT_NAME} requires an out of source build. Use build.sh")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11")

# Not using full-scans anymore at storage/query layer to process records, product length in collection is unused
#add_definitions(-DINDEX_PRODUCT_LENGTH_IN_COLLECTION)

#set(EXTERNAL_PROJECTS_DIR ${PROJECT_SOURCE_DIR}/ext)
#set(EWAHBOOLARRAY_DIR ${EXTERNAL_PROJECTS_DIR}/EWAHBoolArray-0.4.3)
#set(SPARSEHASH_DIR ${EXTERNAL_PROJECTS_DIR}/sparsehash-2.0.2)
#set(RAPIDJSON_DIR ${EXTERNAL_PROJECTS_DIR}/rapidjson)
#set(CMDLINE_DIR ${EXTERNAL_PROJECTS_DIR}/cmdline)
#set(MURMURHASH_DIR ${EXTERNAL_PROJECTS_DIR}/MurmurHash)
#set(LRUCACHE_DIR ${EXTERNAL_PROJECTS_DIR}/lru-cache)

set(PROJECT_TOP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_QUERY_CONTROLLER_DIR ${PROJECT_SOURCE_DIR}/query_controller)
set(PROJECT_UTILS_DIR ${PROJECT_SOURCE_DIR}/utils)
#set(SHERLOCK_TYPES_DIR ${PROJECT_SOURCE_DIR}/thrift)
#set(DATA_DIR ${PROJECT_SOURCE_DIR}/data)
#set(STORAGE_ENGINE_DIR ${PROJECT_SOURCE_DIR}/storage_engine)
#set(QUERY_ENGINE_DIR ${PROJECT_SOURCE_DIR}/query_engine)
#set(CLUSTER_DIR ${PROJECT_SOURCE_DIR}/cluster)
#set(TOOLS_DIR ${PROJECT_SOURCE_DIR}/tools)
#set(MARINA_DIR ${PROJECT_SOURCE_DIR}/marina)

#add_subdirectory(${PROJECT_COMMON_DIR})
add_subdirectory(${PROJECT_QUERY_CONTROLLER_DIR})
add_subdirectory(${PROJECT_UTILS_DIR})
#add_subdirectory(${SHERLOCK_TYPES_DIR})
#add_subdirectory(${DATA_DIR})
#add_subdirectory(${STORAGE_ENGINE_DIR})
#add_subdirectory(${QUERY_ENGINE_DIR})
#add_subdirectory(${CLUSTER_DIR})
#add_subdirectory(${TOOLS_DIR})

#add_subdirectory(${MURMURHASH_DIR})

#----------------------------------------------------------
# Enable testing for whole project
#----------------------------------------------------------
enable_testing()
