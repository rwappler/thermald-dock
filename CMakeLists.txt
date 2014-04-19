project(thermald-dock)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_VERBOSE_MAKEFILE ON)
include(CTest)
enable_testing()

set(CMAKE_BUILD_TYPE DEBUG)

add_subdirectory(src bin)
