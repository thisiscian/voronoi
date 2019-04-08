cmake_minimum_required(VERSION 3.1)
project(voronoi)

find_package(Boost REQUIRED COMPONENTS python37)

AUX_SOURCE_DIRECTORY(src SOURCES)
AUX_SOURCE_DIRECTORY(includes INCLUDES)

list(REMOVE_ITEM SOURCES "src/python.cpp")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(PYTHON_DOT_VERSION 3.7)
set(PYTHON_INCLUDE /usr/include/python3.7m)
set(PYTHON_LIBRARY /usr/lib/libpython3.7m.so)
include_directories("include/" ${PYTHON_INCLUDE} ${Boost_INCLUDE_DIRS})

add_executable(voronoi ${SOURCES} ${INCLUDES})
target_include_directories(voronoi PRIVATE include)

add_library(pyvoronoi SHARED ${SOURCES} ${INCLUDES} src/python.cpp)
set_target_properties(pyvoronoi PROPERTIES PREFIX "")
TARGET_LINK_LIBRARIES(pyvoronoi ${Boost_LIBRARIES} -lpython3.7m -fPIC -shared)
file(COPY bin/ DESTINATION ${CMAKE_BINARY_DIR}/bin) 
