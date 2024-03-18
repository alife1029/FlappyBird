#include "engine_pch.h"
#include "Texture2D.h"

namespace Engine
{
	Texture2D* Texture2D::LoadWhiteTexture()
	{
		unsigned char pixelData[] = { 255, 255, 255 };
		return new Texture2D(pixelData, 1, 1, 3, 1, Filter::Point, Wrap::Clamp);
	}

	Texture2D::Texture2D(unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit, Filter filter, Wrap wrap)
		:
		m_Width(width), m_Height(height), m_ChannelCount(channelCount), m_PixelPerUnit(pixelPerUnit), m_Filter(filter), m_Wrap(wrap)
	{
		GenerateTextureFromBytes(pixels, width, height, channelCount, filter, wrap);
	}

	Texture2D::Texture2D(const std::string& imageFile, unsigned int pixelPerUnit, bool forceRGBA, Filter filter, Wrap wrap)
	{
		int desiredChannelCount = forceRGBA ? STBI_rgb_alpha : STBI_default;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* pixels = stbi_load(imageFile.c_str(), &m_Width, &m_Height, &m_ChannelCount, desiredChannelCount);

		GenerateTextureFromBytes(pixels, m_Width, m_Height, m_ChannelCount, filter, wrap);

		stbi_image_free(pixels);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void Texture2D::Bind() const noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	int Texture2D::GetWidth() const noexcept
	{
		return m_Width;
	}
	int Texture2D::GetHeight() const noexcept
	{
		return m_Height;
	}
	int Texture2D::ChannelCount() const noexcept
	{
		return m_ChannelCount;
	}
	unsigned int Texture2D::PixelPerUnit() const noexcept
	{
		return m_PixelPerUnit;
	}
	Texture2D::Filter Texture2D::GetFilterMode() const noexcept
	{
		return m_Filter;
	}
	Texture2D::Wrap Texture2D::GetWrapMode() const noexcept
	{
		return m_Wrap;
	}
	uint32_t Texture2D::GetID() const noexcept
	{
		return m_TextureID;
	}

	void Texture2D::SetPixelPerUnit(int ppu) noexcept
	{
		m_PixelPerUnit = ppu;
	}
	void Texture2D::ChangeFilterMode(Filter f) noexcept
	{
		if (m_Filter != f)
		{
			constexpr GLint filterIDs[] = { GL_NEAREST, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR };
			
			Bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterIDs[static_cast<int>(f)]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterIDs[static_cast<int>(f)]);
			
			m_Filter = f;
		}
	}
	void Texture2D::ChangeWrapMode(Wrap w) noexcept
	{
		if (m_Wrap != w)
		{
			constexpr GLint wrapIDs[] = { GL_REPEAT, GL_CLAMP_TO_EDGE };

			Bind();

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapIDs[static_cast<int>(w)]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapIDs[static_cast<int>(w)]);

			m_Wrap = w;
		}
	}

	void Texture2D::GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp)
	{
		constexpr GLint imgFormats[] = { GL_RED, GL_RG, GL_RGB, GL_RGBA };
		GLint imgFormat = NULL;

		if (ch >= 1 && ch <= 4) imgFormat = imgFormats[ch - 1];
		else std::cout << "Unsupported image format! (" << ch << " channels)" << std::endl;	// TODO: Throw an exception

		glGenTextures(1, &m_TextureID);
		Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, imgFormat, w, h, NULL, imgFormat, GL_UNSIGNED_BYTE, pixels);
		ChangeFilterMode(f);
		ChangeWrapMode(wrp);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}
