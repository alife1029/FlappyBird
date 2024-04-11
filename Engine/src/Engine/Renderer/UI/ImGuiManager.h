#pragma once

#include "Engine/App/Window.h"

namespace Engine
{
	class ImGuiManager
	{
	public:
		static void Initialize(Window* window);
		static void Shutdown();

		static void NewFrame();
		static void EndFrame();
	};
}
