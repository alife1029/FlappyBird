#include "engine_pch.h"
#include "Viewport.h"

namespace Engine
{
	Viewport::Viewport(Window* targetWindow)
		:
		m_TargetWindow(targetWindow), m_X(0), m_Y(0), m_W(640), m_H(480)
	{
	}

	Viewport::Viewport(Window* targetWindow, int x, int y, int w, int h)
		:
		m_TargetWindow(targetWindow), m_X(x), m_Y(y), m_W(w), m_H(h)
	{
	}

	void Viewport::Apply() const noexcept
	{
		// TODO: Implement Direct3D11 Viewport

		switch (m_TargetWindow->GetGfx()->GetAPI())
		{
		case Graphics::Api::OPENGL: glViewport(m_X, m_Y, m_W, m_H); break;
		}
	}

	int Viewport::GetX() const noexcept
	{
		return m_X;
	}
	int Viewport::GetY() const noexcept
	{
		return m_Y;
	}
	int Viewport::GetW() const noexcept
	{
		return m_W;
	}
	int Viewport::GetH() const noexcept
	{
		return m_H;
	}
	float Viewport::GetAspectRatio() const noexcept
	{
		return static_cast<float>(m_W) / m_H;
	}

	void Viewport::SetDimensions(int x, int y, int w, int h)
	{
		m_X = x;
		m_Y = y;
		m_W = w;
		m_H = h;
	}
}
