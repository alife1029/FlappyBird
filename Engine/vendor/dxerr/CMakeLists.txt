cmake_minimum_required(VERSION 3.5)
project(dxerr)

add_library(${PROJECT_NAME}	src/dxerr.h
							src/dxerr.cpp
							src/DXGetErrorDescription.inl
							src/DXGetErrorString.inl
							src/DXTrace.inl)
target_include_directories(${PROJECT_NAME} PRIVATE "src")
