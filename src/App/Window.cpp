#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int				Window::s_Width = 0, 
				Window::s_Height = 0;
std::string		Window::s_Title = "";
bool			Window::s_FullScreen = false,
				Window::s_GLFWinitialized = false;
GLFWwindow*		Window::s_WindowHandle = nullptr;

void Window::Create(int width, int height, const std::string& title, bool fullScreen)
{
	if (!s_GLFWinitialized)
	{
		if (!glfwInit())
		{
			// TODO: Throw exception
			std::cout << "Failed to initialize GLFW!" << std::endl;
			return;
		}

		s_GLFWinitialized = true;
	}

	s_WindowHandle = glfwCreateWindow(width, height, title.c_str(), fullScreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (!s_WindowHandle)
	{
		// TODO: Throw exception
		std::cout << "Failed to create GLFW window!" << std::endl;
		return;
	}

	glfwMakeContextCurrent(s_WindowHandle);

	// Load Modern OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// TODO: Throw exception
		std::cout << "Failed to load Modern OpenGL!" << std::endl;
		glfwDestroyWindow(s_WindowHandle);
		s_WindowHandle = nullptr;
		return;
	}
	
	// V-Sync
	glfwSwapInterval(1);

	// Set window callbacks
	glfwSetFramebufferSizeCallback(s_WindowHandle, glfw_framebuffer_size_callback);

	// Set variables
	s_Width = width;
	s_Height = height;
	s_Title = title;
	s_FullScreen = fullScreen;
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(s_WindowHandle);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

bool Window::IsOpen()
{
	return glfwWindowShouldClose(s_WindowHandle) == 0;
}

int Window::GetWidth()
{
	return s_Width;
}

int Window::GetHeight()
{
	return s_Height;
}

std::pair<int, int> Window::GetSize()
{
	return { s_Width, s_Height };
}

void Window::SetWidth(int width)
{
	glfwSetWindowSize(s_WindowHandle, width, s_Height);
}

void Window::SetHeight(int height)
{
	glfwSetWindowSize(s_WindowHandle, s_Width, height);
}

void Window::SetSize(int width, int height)
{
	glfwSetWindowSize(s_WindowHandle, width, height);
}

void Window::SetTitle(const std::string& title)
{
	glfwSetWindowTitle(s_WindowHandle, title.c_str());
	s_Title = title;
}

std::string Window::GetTitle()
{
	return s_Title;
}

void Window::glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	s_Width = width;
	s_Height = height;
}
