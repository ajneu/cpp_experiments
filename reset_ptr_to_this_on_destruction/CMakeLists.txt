cmake_minimum_required(VERSION 2.8.11)

##############
# C++ Standard
##############
if (NOT    CMAKE_VERSION VERSION_LESS 3.1)
  set(CMAKE_CXX_STANDARD   14)
  set(CMAKE_CXX_EXTENSIONS OFF)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()


project(reset_ptr_to_this)
set(target go)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)


##############
# Warnings
##############
if ( CMAKE_COMPILER_IS_GNUCC )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif ( CMAKE_COMPILER_IS_GNUCC )


##############
# Sources
##############
file (GLOB cplusplus_SRC "*.cpp" "include/*.h")
include_directories ("." "include")

add_executable(${target} ${cplusplus_SRC})


##############
# Linking
##############


## Boost
find_package(Boost COMPONENTS system REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

## PThread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)



target_link_libraries(${target} ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
