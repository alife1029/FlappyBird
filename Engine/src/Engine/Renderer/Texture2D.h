#pragma once

#include <string>
#include <cstdint>

namespace Engine
{
	class Texture2D
	{
	public:
		enum class Filter { Point = 0, Bilinear = 1, Trilinear = 2 };
		enum class Wrap { Repeat = 0, Clamp = 1 };

	public:
		static Texture2D* LoadWhiteTexture();

	public:
		Texture2D(unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit = 100u, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);
		Texture2D(const std::string& imageFile, unsigned int pixelPerUnit = 100u, bool forceRGBA = false, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		virtual ~Texture2D();

		virtual void Bind() const noexcept;

		int GetWidth() const noexcept;
		int GetHeight() const noexcept;
		int ChannelCount() const noexcept;
		unsigned int PixelPerUnit() const noexcept;
		Filter GetFilterMode() const noexcept;
		Wrap GetWrapMode() const noexcept;

		void SetPixelPerUnit(int ppu) noexcept;
		virtual void ChangeFilterMode(Filter f) noexcept;
		virtual void ChangeWrapMode(Wrap w) noexcept;

	protected:
		virtual void GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp);

	protected:
		int				m_Width,
						m_Height,
						m_ChannelCount;
		unsigned int	m_PixelPerUnit;
		Filter			m_Filter;
		Wrap			m_Wrap;
	};
}
