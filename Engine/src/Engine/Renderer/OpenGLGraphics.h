#pragma once

#include "Engine/Utils/EngineException.h"

#ifdef ENGINE_PLATFORM_WINDOWS	
#include <Windows.h>
#endif

namespace Engine
{
	class Window;

	class OpenGLGraphics
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
		OpenGLGraphics(Window* targetWindow);
		OpenGLGraphics(const OpenGLGraphics&) = delete;
		OpenGLGraphics& operator=(const OpenGLGraphics&) = delete;
		~OpenGLGraphics();

		void EndFrame();
		void ClearBuffer(float red, float green, float blue, float alpha) noexcept;
		void DrawIndexed(unsigned int count);

	private:
#ifdef ENGINE_PLATFORM_WINDOWS	
		HGLRC	m_Context;
		HDC		m_Device;
#endif
		Window*	m_TargetWindow;
	};
}
