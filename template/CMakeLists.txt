cmake_minimum_required(VERSION 3.5.2)

set(PROJECT_NAME template)
set(ARTIFACTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/artifacts)

if(NOT WIN32)
  set(CMAKE_C_COMPILER    clang)
  set(CMAKE_CXX_COMPILER  clang++)
  set(CMAKE_C_FLAGS       "-Wall -std=c99")
  set(CMAKE_C_FLAGS_DEBUG "-g")
endif(NOT WIN32)

set(CMAKE_CXX_LINK_FLAGS  "-lstdc++")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(CMAKE_C_STANDARD 99)

project (template LANGUAGES CXX C)
add_subdirectory(src)
