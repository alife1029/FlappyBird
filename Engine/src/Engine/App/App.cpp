#include "engine_pch.h"

#include "App.h"
#include "Engine/Utils/Time.h"

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
}
