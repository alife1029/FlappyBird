cmake_minimum_required(VERSION 3.5)
project(glad VERSION 4.6)

add_library(${PROJECT_NAME} src/glad.c)
target_include_directories(${PROJECT_NAME} PRIVATE "include")
