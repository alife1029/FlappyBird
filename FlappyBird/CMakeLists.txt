cmake_minimum_required(VERSION 3.5)

project(FlappyBird VERSION 0.0.1)

file(GLOB FLAPPY_PROJECT_SOURCES	src/**.h
									src/**.cpp)

add_executable(${PROJECT_NAME} WIN32 ${FLAPPY_PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE	../Engine/src
													../Engine/vendor/glm)
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)
