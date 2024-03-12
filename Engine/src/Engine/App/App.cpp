#include "App.h"
#include "Engine/Utils/Time.h"

namespace Engine
{
	App::App() 
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
