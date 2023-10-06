cmake_minimum_required(VERSION 3.5)

include (cmake/libs.cmake)

project(FlappyBird VERSION 0.0.1)

file(GLOB PROJECT_SOURCE_FILES	${CMAKE_SOURCE_DIR}/src/**.cpp
								${CMAKE_SOURCE_DIR}/src/App/**.h
								${CMAKE_SOURCE_DIR}/src/App/**.cpp
								)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBS})
