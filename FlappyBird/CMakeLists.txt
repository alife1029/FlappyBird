cmake_minimum_required(VERSION 3.5)

project(FlappyBird VERSION 0.0.1)

file(GLOB FLAPPY_PROJECT_SOURCES	src/**.h
									src/**.cpp
									src/**.rc)

add_executable(${PROJECT_NAME} WIN32 ${FLAPPY_PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE	../Engine/src
													../Engine/vendor/glm
													../Engine/vendor/imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE Engine opengl32)

# Post Build Commands
add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/res/
					${CMAKE_BINARY_DIR}/${PROJECT_NAME}/res/
					)
add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/Engine/res/
					${CMAKE_BINARY_DIR}/${PROJECT_NAME}/res/
					)
