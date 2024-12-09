cmake_minimum_required(VERSION 3.30)
project(imgui VERSION 4.6)

set(IMGUI_SOURCES	imgui.cpp imgui_demo.cpp 
					imgui_draw.cpp imgui_impl_opengl3.cpp )

if (WIN32)
	list(APPEND IMGUI_SOURCES	imgui_impl_win32.cpp 
								imgui_tables.cpp 
								imgui_widgets.cpp)
elseif (UNIX)
	list(APPEND IMGUI_SOURCES	imgui_impl_glfw.cpp)
endif()

add_library(${PROJECT_NAME} ${IMGUI_SOURCES})
