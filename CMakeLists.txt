cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)

project(TakeHome-Prj)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_TESTS_UNIT "build unit tests" OFF)

add_subdirectory(src)

set(APP_NAME Balancer)
add_executable(${APP_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/Main.cc)
target_link_libraries(${APP_NAME} PUBLIC Balancer-lib)
