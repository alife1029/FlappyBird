#include "engine_pch.h"
#include "Input.h"

namespace Engine
{
	static Window* s_EventWindow = nullptr;

	bool Input::IsKeyPressed(Key key) noexcept
	{
		return s_EventWindow->m_Keyboard.IsKeyPressed(static_cast<unsigned char>(key));
	}

	bool Input::IsKeyPressed(unsigned char key) noexcept
	{
		return s_EventWindow->m_Keyboard.IsKeyPressed(key);
	}
	
	std::vector<unsigned char> Input::GetPressingKeys() noexcept
	{
		std::vector<unsigned char> pressedKeys;

		for (unsigned short i = 0; i < s_EventWindow->m_Keyboard.s_KeyCount; i++)
		{
			if (s_EventWindow->m_Keyboard.IsKeyPressed(static_cast<unsigned char>(i)))
			{
				pressedKeys.push_back(static_cast<unsigned char>(i));
			}
		}

		return pressedKeys;
	}

	void Input::SetupEventWindow(Window* eventWnd) noexcept
	{
		s_EventWindow = eventWnd;
	}
}
