#include "engine_pch.h"
#include "GLFWWindow.h"

namespace Engine
{
	bool GLFWWindow::s_GLFWinitialized = false;

	GLFWWindow::GLFWWindow(int width, int height, const std::string& title, bool fullScreen)
		:
		Window(width, height, title, fullScreen)
	{
		if (!s_GLFWinitialized)
		{
			// Initialize GLFW
			if (!glfwInit())
			{
				// TODO: Throw error
			}

			s_GLFWinitialized = true;
		}

		// TODO: Implement fullscreen
		m_Window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	}

	GLFWWindow::~GLFWWindow()
	{
		glfwDestroyWindow(m_Window);
	}

	void GLFWWindow::CreateGraphicsContext()
	{
		m_Graphics = new OpenGLGraphics(this);
	}
    
	void GLFWWindow::ProcessEvents()
    {
		glfwPollEvents();
    }
    
	GLFWwindow *GLFWWindow::GetGLFWwindow()
    {
        return m_Window;
    }
}
