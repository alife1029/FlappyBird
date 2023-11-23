#pragma once

class Viewport
{
public:
	Viewport(int width, int height);
	Viewport(int x, int y, int width, int height);
	virtual void Apply();
	void SetX(int x) noexcept { m_X = x; }
	void SetY(int y) noexcept { m_Y = y; }
	void SetWidth(int width) noexcept { m_Width = width; }
	void SetHeight(int height) noexcept { m_Height = height; }
	int GetX() const noexcept { return m_X; }
	int GetY() const noexcept { return m_Y; }
	int GetWidth() const noexcept { return m_Width; }
	int GetHeight() const noexcept { return m_Height; }
	float GetAspectRatio() const noexcept { return static_cast<float>(m_Width) / static_cast<float>(m_Height); }
protected:
	int	m_X,
		m_Y,
		m_Width,
		m_Height;
};
