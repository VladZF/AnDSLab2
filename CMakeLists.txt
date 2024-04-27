cmake_minimum_required(VERSION 3.28)
project(AlgoLab2)

set(CMAKE_CXX_STANDARD 17)

add_executable(AlgoLab2 main.cpp
        Algorithms/TreeAlgorithm.h
)

target_include_directories(AlgoLab2 PRIVATE Algorithms)
