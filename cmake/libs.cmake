add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/stb)

set(INCLUDE_DIRS	vendor/glfw/include
					vendor/glad/include
					vendor/glm
					vendor/stb/include
					)

set(LINK_LIBS	glfw
				glad
				stb
				)
