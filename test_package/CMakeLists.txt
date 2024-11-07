cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(PackageTest CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(cppte REQUIRED COMPONENTS Devel)
find_package(spdlog)

add_executable(example example.cpp shape.cpp)
target_link_libraries(example PRIVATE cppte::cppte spdlog::spdlog
)
