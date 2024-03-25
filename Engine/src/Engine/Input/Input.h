#pragma once

#include "Key.h"
#include "Engine/App/Window.h"

#include <vector>

namespace Engine
{
	class Input
	{
		friend class App;
	public:
		static bool IsKeyPressed(Key key) noexcept;
		static bool IsKeyPressed(unsigned char key) noexcept;
		static bool IsKeyJustPressed(Key key) noexcept;
		static bool IsKeyJustPressed(unsigned char key) noexcept;
		static std::vector<unsigned char> GetPressingKeys() noexcept;
		static void SetupEventWindow(Window* eventWnd) noexcept;
	};
}
