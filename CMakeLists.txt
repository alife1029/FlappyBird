cmake_minimum_required(VERSION 3.5)

project(FlappyBird VERSION 0.0.1)

file(GLOB PROJECT_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/**.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
