cmake_minimum_required(VERSION 3.30)

project(Engine VERSION 1.0.0)

set(ENIGNE_DEPS)

# Libraries
add_subdirectory(vendor/glad)
add_subdirectory(vendor/stb)
add_subdirectory(vendor/imgui)
list(APPEND ENGINE_DEPS glad stb opengl32 imgui)
if (WIN32)
	file (GLOB ENGINE_PLATFORM_SOURCES	src/Engine/Platform/Win32/**.h
										src/Engine/Platform/Win32/**.cpp)
	list(APPEND ENGINE_DEPS	${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/freetype/lib/freetype.lib
							${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/openal/lib/OpenAL32.lib
							${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/vendor/sndfile/lib/sndfile.lib)
elseif (UNIX)
	file (GLOB ENGINE_PLATFORM_SOURCES	src/Engine/Platform/Linux/**.h
										src/Engine/Platform/Linux/**.cpp)
	find_package(glfw3 3.4 REQUIRED)
	find_package(OpenAL REQUIRED)
	list(APPEND ENGINE_DEPS glfw OpenAL)
endif()

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

set(ENGINE_INCLUDE_DIRS	vendor/glad/include
						vendor/stb/include
						vendor/glm
						vendor/imgui
						vendor/sndfile/include
						src/)

if (WIN32)
	list(APPEND ENGINE_INCLUDE_DIRS	vendor/freetype/include
									vendor/openal/include)
elseif (UNIX)
	list(APPEND ENGINE_INCLUDE_DIRS	/usr/include/freetype2)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${ENGINE_INCLUDE_DIRS})

target_link_directories(${PROJECT_NAME} PRIVATE	vendor/freetype/lib
												vendor/openal/lib
												vendor/sndfile/lib)

target_link_libraries(${PROJECT_NAME} PRIVATE ${ENGINE_DEPS})
												
target_precompile_headers(${PROJECT_NAME} PRIVATE src/engine_pch.h)

# Preprocessor macro predefinitios
if (WIN32)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -DENGINE_PLATFORM_WINDOWS)
elseif (UNIX)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -DENGINE_PLATFORM_UNIX)
endif()

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
