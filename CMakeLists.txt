cmake_minimum_required(VERSION 2.6)

project(Yan)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS
        -g
        -march=native
        -rdynamic
        -std=c++11
        )

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O2")

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

file(GLOB_RECURSE SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Network/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Network/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Common/*.cc
        )

add_library(Yan ${SOURCE})
target_link_libraries(Yan pthread rt)
set_target_properties(Yan PROPERTIES OUTPUT_NAME yohub)
install(TARGETS Yan DESTINATION lib)

add_subdirectory(demo)