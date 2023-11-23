#pragma once

#include <string>

struct GLFWwindow;

class Window
{
public:
	Window(int width, int height, const std::string& title, bool fullScreen = false);
	Window& operator=(const Window&) = delete;
	~Window();
	operator GLFWwindow* ();
	void SwapBuffers();
	void PollEvents();
	bool IsOpen();
	int GetWidth();
	int GetHeight();
	std::pair<int, int> GetSize();
	void SetWidth(int width);
	void SetHeight(int height);
	void SetSize(int width, int height);
	std::string GetTitle();
	void SetTitle(const std::string& title);

private:
	int			m_Width, 
				m_Height;

	std::string	m_Title;

	bool		m_FullScreen;
	static bool	s_GLFWinitialized,
				s_GLADinitialized;

	GLFWwindow*	m_WindowHandle;

private:
	static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
