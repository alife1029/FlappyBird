cmake_minimum_required(VERSION 3.5)

project(Engine VERSION 1.0.0)

# Libraries
add_subdirectory(vendor/glad)
add_subdirectory(vendor/stb)
add_subdirectory(vendor/imgui)

# Source files
file (GLOB ENGINE_PROJECT_SOURCES	src/**.h
									src/Engine/App/**.h
									src/Engine/App/**.cpp
									src/Engine/Audio/**.h
									src/Engine/Audio/**.cpp
									src/Engine/Renderer/**.h
									src/Engine/Renderer/**.cpp
									src/Engine/Renderer/UI/**.h
									src/Engine/Renderer/UI/**.cpp
									src/Engine/Utils/**.h
									src/Engine/Utils/**.cpp
									src/Engine/Input/**.h
									src/Engine/Input/**.cpp)

add_library(${PROJECT_NAME} ${ENGINE_PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE	vendor/glad/include
													vendor/stb/include
													vendor/glm
													vendor/freetype/include
													vendor/imgui
													vendor/openal/include
													vendor/sndfile/include
													src/)

target_link_directories(${PROJECT_NAME} PRIVATE	vendor/freetype/lib
												vendor/openal/lib
												vendor/sndfile/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE	glad 
												stb
												opengl32
												imgui
												${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/freetype/lib/freetype.lib
												${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/openal/lib/OpenAL32.lib
												${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/sndfile/lib/sndfile.lib)
												
target_precompile_headers(${PROJECT_NAME} PRIVATE src/engine_pch.h)

# Post Build Commands
add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/res/
					${CMAKE_BINARY_DIR}/${PROJECT_NAME}/res/

					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/openal/dll/
					${CMAKE_BINARY_DIR}/${PROJECT_NAME}/dll/

					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/sndfile/dll/
					${CMAKE_BINARY_DIR}/${PROJECT_NAME}/dll/
					)
