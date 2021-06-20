cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17 -Wall -O2)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(Mandelbrot)

add_executable(${PROJECT_NAME} src/main.cpp src/Viewer.cpp src/Fractal.cpp src/Point.cpp)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} pthread)
