#include "engine_pch.h"

#include "Engine/App/AppManager.h"

#include <imgui.h>
#include <imgui_impl_win32.h>

#define	ENGINE_WND_EXCEPT(hr) Engine::Window::HrException(__LINE__, __FILE__, hr)
#define ENGINE_WND_LASTEXCEPT() Engine::Window::HrException(__LINE__, __FILE__, GetLastError())

namespace Engine
{
	Window::Window(int width, int height, const std::string& title, bool fullScreen)
		:
		m_Width(width), m_Height(height), m_title(title), m_IsFullscreen(fullScreen), m_Graphics(nullptr), m_HasFocus(false), m_IsShown(false)
	{
	}

	Window::~Window()
	{	
	}

	void Window::SetWidth(int width)
	{
		// TODO: Implement this
	}
	void Window::SetHeight(int height)
	{
		// TODO: Implement this
	}
	void Window::SetTitle(const std::string& title)
	{
		
	}
	void Window::SetFullscreenState(bool fullScreen)
	{
		// TODO: Implement this
	}
	void Window::Show()
	{
		m_IsShown = true;
	}
	void Window::Hide() 
	{
		m_IsShown = false;
	}
	int Window::GetWidth() const noexcept
	{
		return m_Width;
	}
	int Window::GetHeight() const noexcept
	{
		return m_Height;
	}
	std::string Window::GetTitle() const noexcept
	{
		return m_title;
	}
	OpenGLGraphics* Window::GetGfx() const noexcept
	{
		return m_Graphics;
	}

	void Window::ProcessEvents()
	{
	}

	void Window::CreateGraphicsContext()
	{
	}
}
