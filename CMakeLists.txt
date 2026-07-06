cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(RoboBuilderSim)

add_executable(robot_builder_simulator src/main.cpp src/engine/engine.cpp src/resource_collectors/resource_collector.cpp src/ingredients/silo.cpp)

target_include_directories(robot_builder_simulator PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)