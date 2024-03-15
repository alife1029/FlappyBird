#pragma once

#include "Engine/Utils/EngineException.h"

#include <Windows.h>

namespace Engine
{
	class Window;

	class OpenGLGraphics
	{
	public:
		class InitializationError : public EngineException
		{
		public:
			InitializationError(int line, const char* file, const std::string& errorDetails) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			std::string GetErrorDetails() const noexcept;
		private:
			std::string m_Details;
		};

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
		HGLRC	m_Context;
		HDC		m_Device;
		Window*	m_TargetWindow;
	};
}
