cmake_minimum_required(VERSION 2.6)
project(Yan CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

file(GLOB_RECURSE SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/Common/*.cc
        ${PROJECT_SOURCE_DIR}/src/Network/*.cc
)

add_library(Yan ${SOURCE_FILES})
install(TARGETS Yan DESTINATION lib)

add_subdirectory(demo/echo)