cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fno-inline -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
if(NOT CMAKE_BUILD_TYPE)
  message(WARNING "No build type specified, defaulting to Debug.")
  set(CMAKE_BUILD_TYPE Debug)
endif()

project(GateServer LANGUAGES CXX)

add_subdirectory(gateserver)
add_subdirectory(message)
