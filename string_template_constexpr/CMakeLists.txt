cmake_minimum_required(VERSION 2.8.11)

project(go)
set(target go)


##############
# C++ Standard
##############
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

##############
# Sources
##############

add_executable(${target} main.cpp)

