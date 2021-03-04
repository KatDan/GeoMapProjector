cmake_minimum_required(VERSION 3.17)
project(GeoMapProjector)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE sources "src/*.cpp")

add_executable(GeoMapProjector ${sources})