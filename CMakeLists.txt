cmake_minimum_required(VERSION 3.5)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 20)

project(cpp-te VERSION 1.0 LANGUAGES CXX)

add_library(cpp-te INTERFACE)
target_include_directories(cpp-te INTERFACE include)

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
