cmake_minimum_required(VERSION 3.10)
project(roguelike)
add_executable(${CMAKE_PROJECT_NAME} main.cpp)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE model)
add_subdirectory(model)
