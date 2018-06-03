cmake_minimum_required(VERSION 2.8)
project(alpha_reversi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp core/reversi.cpp core/reversi.h core/uct.h core/uct.cpp core/timer.h)
add_executable(alpha_reversi ${SOURCE_FILES})
