add_subdirectory(libs/glfw)
add_subdirectory(libs/glad)
add_subdirectory(libs/stb)

set(INCLUDE_DIRS	libs/glfw/include
					libs/glad/include
					libs/glm
					libs/stb/include
					)

set(LINK_LIBS	glfw
				glad
				stb
				)
