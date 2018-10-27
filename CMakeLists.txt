cmake_minimum_required(VERSION 3.12)
project(A02_pgm)

#set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

add_executable(A02_pgm main.cpp)