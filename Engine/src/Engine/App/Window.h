#pragma once

#include "Engine/Utils/EngineException.h"
#include "Engine/Renderer/OpenGLGraphics.h"
#include "Engine/Input/Keyboard.h"

namespace Engine
{
	class Window
	{
		friend class Input;
	public:
		Window(int width, int height, const std::string& title, bool fullScreen);
		virtual ~Window();

		virtual void SetWidth(int width);
		virtual void SetHeight(int height);
		virtual void SetTitle(const std::string& title);
		virtual void SetFullscreenState(bool fullScreen);
		virtual void Show();
		virtual void Hide();

		int GetWidth() const noexcept;
		int GetHeight() const noexcept; 
		virtual std::string GetTitle() const noexcept;
		OpenGLGraphics* GetGfx() const noexcept;

		virtual void ProcessEvents();
		virtual void CreateGraphicsContext();

	private:
		int m_Width, m_Height;
		std::string m_title;
		bool	m_IsFullscreen, 
				m_IsShown,
				m_HasFocus;

	protected:
		OpenGLGraphics* m_Graphics;
		Keyboard m_Keyboard;
	};
}
