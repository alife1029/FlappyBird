#pragma once

namespace Engine
{
	class Window;
	class Viewport
	{
	public:
		Viewport(Window* targetWindow);
		Viewport(Window* targetWindow, int x, int y, int w, int h);

		virtual void Bind() noexcept;

		int GetX() const noexcept;
		int GetY() const noexcept;
		int GetW() const noexcept;
		int GetH() const noexcept;
		float GetAspectRatio() const noexcept;

		void SetDimensions(int x, int y, int w, int h);

	protected:
		int	m_X,
			m_Y,
			m_W,
			m_H;
		Window* m_TargetWindow;
	};
}
