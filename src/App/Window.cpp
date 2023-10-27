#include "Window.h"
#include "../Utils/Exception.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

bool Window::s_GLFWinitialized = false;
bool Window::s_GLADinitialized = false;

Window::Window(int width, int height, const std::string& title, bool fullScreen)
{
	if (!s_GLFWinitialized)
	{
		if (!glfwInit())
		{
			THROW_INITIALIZATION_EXCEPTION("GLFW");
		}

		s_GLFWinitialized = true;
	}

	m_WindowHandle = glfwCreateWindow(width, height, title.c_str(), fullScreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (!m_WindowHandle)
	{
		THROW_INITIALIZATION_EXCEPTION("Window");
	}

	glfwMakeContextCurrent(m_WindowHandle);

	// Load Modern OpenGL
	if (!s_GLADinitialized)
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			glfwDestroyWindow(m_WindowHandle);
			m_WindowHandle = nullptr;
			THROW_INITIALIZATION_EXCEPTION("OpenGL");
		}

		s_GLADinitialized = true;
	}
	
	// V-Sync
	glfwSwapInterval(1);

	// Set this class as user pointer
	glfwSetWindowUserPointer(m_WindowHandle, reinterpret_cast<void*>(this));

	// Set window callbacks
	glfwSetFramebufferSizeCallback(m_WindowHandle, glfw_framebuffer_size_callback);

	// Set variables
	m_Width = width;
	m_Height = height;
	m_Title = title;
	m_FullScreen = fullScreen;
}

Window::~Window()
{
	glfwDestroyWindow(m_WindowHandle);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

bool Window::IsOpen()
{
	return glfwWindowShouldClose(m_WindowHandle) == 0;
}

int Window::GetWidth()
{
	return m_Width;
}

int Window::GetHeight()
{
	return m_Height;
}

std::pair<int, int> Window::GetSize()
{
	return { m_Width, m_Height };
}

void Window::SetWidth(int width)
{
	glfwSetWindowSize(m_WindowHandle, width, m_Height);
}

void Window::SetHeight(int height)
{
	glfwSetWindowSize(m_WindowHandle, m_Width, height);
}

void Window::SetSize(int width, int height)
{
	glfwSetWindowSize(m_WindowHandle, width, height);
}

void Window::SetTitle(const std::string& title)
{
	glfwSetWindowTitle(m_WindowHandle, title.c_str());
	m_Title = title;
}

std::string Window::GetTitle()
{
	return m_Title;
}

void Window::glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Window* windowObj = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	windowObj->m_Width = width;
	windowObj->m_Height = height;
}
