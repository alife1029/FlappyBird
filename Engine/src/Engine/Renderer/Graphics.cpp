#include "Graphics.h"
#include "Engine/App/Window.h"

namespace Engine
{
	Graphics::Graphics(Window* targetWindow)
		: m_TargetWindow(targetWindow), m_RendererAPI(Api::NONE)
	{
	}

	Graphics::~Graphics()
	{
	}

	void Graphics::EndFrame()
	{
	}

	void Graphics::ClearBuffer(float red, float green, float blue, float alpha) noexcept
	{
	}

	void Graphics::DrawIndexed(unsigned int count)
	{
	}

	Graphics::Api Graphics::GetAPI()
	{
		return m_RendererAPI;
	}
}
