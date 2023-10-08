cmake_minimum_required(VERSION 3.5)

include (cmake/libs.cmake)

project(FlappyBird VERSION 0.0.1)

file(GLOB PROJECT_SOURCE_FILES	${CMAKE_SOURCE_DIR}/src/**.cpp
								${CMAKE_SOURCE_DIR}/src/App/**.h
								${CMAKE_SOURCE_DIR}/src/App/**.cpp
								${CMAKE_SOURCE_DIR}/src/Renderer/**.h
								${CMAKE_SOURCE_DIR}/src/Renderer/**.cpp
								${CMAKE_SOURCE_DIR}/src/Utils/**.h
								${CMAKE_SOURCE_DIR}/src/Utils/**.cpp
								)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})

add_custom_command(	TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
						${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res
					)
