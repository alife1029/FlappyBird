#pragma once

namespace Engine
{
	class Viewport
	{
	public:
		Viewport();
		Viewport(int x, int y, int w, int h);

		void Apply() const noexcept;

		int GetX() const noexcept;
		int GetY() const noexcept;
		int GetW() const noexcept;
		int GetH() const noexcept;
		float GetAspectRatio() const noexcept;

		void SetDimensions(int x, int y, int w, int h);

	private:
		int	m_X,
			m_Y,
			m_W,
			m_H;
	};
}
