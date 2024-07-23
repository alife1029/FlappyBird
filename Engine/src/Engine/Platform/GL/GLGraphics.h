#pragma once

#include "Engine/Renderer/Graphics.h"
#include "Engine/Utils/EngineException.h"

#include <Windows.h>

namespace Engine
{
	class Window;

	class GLGraphics : public Graphics
	{
	public:
		class FramebufferSwapError : public EngineException
		{
			using EngineException::EngineException;
		public:
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
		};

	public:
		GLGraphics(Window* targetWindow);
		GLGraphics(const GLGraphics&) = delete;
		GLGraphics& operator=(const GLGraphics&) = delete;
		~GLGraphics();

		void EndFrame();
		void ClearBuffer(float red, float green, float blue, float alpha) noexcept;
		void DrawIndexed(unsigned int count);

	private:
		HGLRC	m_Context;
		HDC		m_Device;
	};
}
