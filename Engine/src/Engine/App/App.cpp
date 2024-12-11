#include "engine_pch.h"

#include "App.h"
#include "Engine/Utils/Time.h"

#ifdef ENGINE_PLATFORM_WINDOWS
#include "Engine/Platform/Win32/Win32Window.h"
#elif defined(ENGINE_PLATFORM_UNIX)
#include "Engine/Platform/Linux/GLFWWindow.h"
#endif

namespace Engine
{
	App::App() 
		:
		m_Window(nullptr)
	{
		
	}

	App::~App() 
	{
	}

	void App::Start() 
	{
	}

	void App::Update() 
	{
		Time::Update();
	}

	Window* App::GetWindow() const noexcept
	{
		return m_Window;
	}

	Window* App::CreateWindow(int width, int height, const std::string &title, bool fullScreen)
	{
#ifdef ENGINE_PLATFORM_WINDOWS
		m_Window = new Win32Window(width, height, title, fullScreen);
#elif defined(ENGINE_PLATFORM_UNIX)
		m_Window = new GLFWWindow(width, height, title, fullScreen);
#endif
		return m_Window;
	}
}
