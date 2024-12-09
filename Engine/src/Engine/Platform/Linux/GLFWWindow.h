#pragma once

#include "Engine/App/Window.h"

struct GLFWwindow;

namespace Engine
{
	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(int width, int height, const std::string& title, bool fullScreen);
		~GLFWWindow();

		void CreateGraphicsContext() override;
		void ProcessEvents() override;

		GLFWwindow* GetGLFWwindow();

	private:
		GLFWwindow* m_Window;
		static bool s_GLFWinitialized;
	};
}
