#include <cstdlib>
#include <cstdint>
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <regex>
#include <map>

#include <glad/glad.h>

#ifdef ENGINE_PLATFORM_WINDOWS
#include <Windows.h>
#include <glad/glad_wgl.h>
#elif ENGINE_PLATFORM_UNIX
#include <GLFW/glfw3.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H
