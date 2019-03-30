cmake_minimum_required(VERSION 3.1)
project(voronoi)

set(SOURCES src/main.cpp include/parabola.h src/parabola.cpp include/voronoi.h src/voronoi.cpp)
set(CMAKE_CXX_STANDARD 11)

add_executable(voronoi ${SOURCES})
target_include_directories(voronoi PRIVATE include)
