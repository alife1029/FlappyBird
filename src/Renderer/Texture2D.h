#pragma once

#include <string>
#include <cstdint>

class Texture2D 
{
public:
	enum class Filter
	{
		None,
		Linear,
		Trilinear
	};

	enum class Wrap
	{
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

public:
	// Creates white texture
	Texture2D();
	// Load texture from image file
	Texture2D(const std::string& imageFile);
	Texture2D& operator=(const Texture2D&) = delete;
	~Texture2D();

	void Activate() const noexcept;

	void SetFilter(Filter filter) noexcept;
	Filter GetFilter() const noexcept { return m_Filter; }
	void SetWrap(Wrap wrap) noexcept;
	Wrap GetWrap() const noexcept { return m_Wrap; }

private:
	void GenerateObjectFromPixels(unsigned char* pixels);

private:
	int			m_Width,
				m_Height,
				m_ChannelCount,
				m_PixelPerUnit = 100;
	
	uint32_t	m_RendererID;

	Filter		m_Filter	= Filter::None;
	Wrap		m_Wrap		= Wrap::ClampToEdge;
};
