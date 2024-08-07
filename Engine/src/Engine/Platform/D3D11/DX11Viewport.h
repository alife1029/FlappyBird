#pragma once

#include "Engine/Renderer/Viewport.h"

namespace Engine
{
	class DX11Viewport : public Viewport
	{
	public:
		DX11Viewport(Window* targetWindow);
		DX11Viewport(Window* targetWindow, int x, int y, int w, int h);

		void Bind() noexcept override;

	private:
		D3D11_VIEWPORT m_Viewport = {};
	};
}
