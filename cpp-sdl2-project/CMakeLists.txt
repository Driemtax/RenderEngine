cmake_minimum_required(VERSION 3.10)
project(cpp-sdl2-project)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(cpp-sdl2-project src/main.cpp src/helpers/math.cpp)

target_link_libraries(cpp-sdl2-project ${SDL2_LIBRARIES})