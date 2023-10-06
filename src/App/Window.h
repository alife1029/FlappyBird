#pragma once

#include <string>

struct GLFWwindow;

class Window
{
public:
	static void CreateWindow(int width, int height, const std::string& title, bool fullScreen = false);
	static void SwapBuffers();
	static void PollEvents();
	static bool IsOpen();
	static int GetWidth();
	static int GetHeight();
	static std::pair<int, int> GetSize();
	static void SetWidth(int width);
	static void SetHeight(int height);
	static void SetSize(int width, int height);
	static std::string GetTitle();
	static void SetTitle(const std::string& title);

private:
	static int			s_Width, 
						s_Height;

	static std::string	s_Title;

	static bool			s_FullScreen,
						s_GLFWinitialized;

	static GLFWwindow*	s_WindowHandle;

private:
	static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
