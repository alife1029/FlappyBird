#include "engine_pch.h"
#include "DX11Viewport.h"
#include "DX11Graphics.h"

#include "Engine/App/Window.h"

namespace Engine
{
	DX11Viewport::DX11Viewport(Window* targetWindow) 
		: 
		Viewport(targetWindow)
	{
	}

	DX11Viewport::DX11Viewport(Window* targetWindow, int x, int y, int w, int h)
		:
		Viewport(targetWindow, x, y, w, h)
	{
	}

	void DX11Viewport::Bind() noexcept
	{
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.Width = static_cast<float>(m_W);
		m_Viewport.Height = static_cast<float>(m_H);
		m_Viewport.TopLeftX = static_cast<float>(m_X);
		m_Viewport.TopLeftY = static_cast<float>(m_Y);

		((DX11Graphics*)m_TargetWindow->GetGfx())->GetContext()->RSSetViewports(1, &m_Viewport);
	}
}
