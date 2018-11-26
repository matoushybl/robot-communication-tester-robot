cmake_minimum_required(VERSION 3.7)
project(BaseStation)

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(BaseStation main.cpp UDP.cpp UDP.h Variable.cpp Variable.h)