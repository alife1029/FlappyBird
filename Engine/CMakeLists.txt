cmake_minimum_required(VERSION 3.5)

project(Engine VERSION 0.0.1)

# Libraries
add_subdirectory(vendor/glad)
add_subdirectory(vendor/stb)

# Source files
file (GLOB ENGINE_PROJECT_SOURCES	src/**.h
									src/Engine/App/**.h
									src/Engine/App/**.cpp
									src/Engine/Renderer/**.h
									src/Engine/Renderer/**.cpp
									src/Engine/Utils/**.h
									src/Engine/Utils/**.cpp)

add_library(${PROJECT_NAME} ${ENGINE_PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE	vendor/glad/include
													vendor/stb/include
													vendor/glm
													src/)
target_link_libraries(${PROJECT_NAME} PRIVATE	glad 
												stb
												opengl32)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/engine_pch.h)