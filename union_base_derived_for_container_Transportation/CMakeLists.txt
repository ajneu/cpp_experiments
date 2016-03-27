cmake_minimum_required(VERSION 2.8.11)

project(go)
set(target go)

if ( CMAKE_COMPILER_IS_GNUCC )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-unused-variable")        # suppress
endif()

##############
# C++ Standard
##############
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

##############
# Sources
##############

add_executable(${target} main.cpp)

