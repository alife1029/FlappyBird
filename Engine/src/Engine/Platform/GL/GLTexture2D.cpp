#include "engine_pch.h"
#include "GLTexture2D.h"
#include "Engine/Utils/EngineException.h"

namespace Engine
{
	GLTexture2D::GLTexture2D(unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit, Filter filter, Wrap wrap)
		:
		Texture2D(pixels, width, height, channelCount, pixelPerUnit, filter, wrap)
	{
		GenerateTextureFromBytes(pixels, width, height, channelCount, filter, wrap);
	}

	GLTexture2D::GLTexture2D(const std::string& imageFile, unsigned int pixelPerUnit, bool forceRGBA, Filter filter, Wrap wrap)
		:
		Texture2D(imageFile, pixelPerUnit, forceRGBA, filter, wrap)
	{
		int desiredChannelCount = forceRGBA ? STBI_rgb_alpha : STBI_default;

		stbi_set_flip_vertically_on_load(true);
		unsigned char* pixels = stbi_load(imageFile.c_str(), &m_Width, &m_Height, &m_ChannelCount, desiredChannelCount);

		if (!pixels)
		{
			// Image not loaded
			throw ResourceNotFoundException(__LINE__, __FILE__, imageFile);
		}

		GenerateTextureFromBytes(pixels, m_Width, m_Height, m_ChannelCount, filter, wrap);

		stbi_image_free(pixels);
		m_PixelPerUnit = pixelPerUnit;
	}

	GLTexture2D::~GLTexture2D()
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void GLTexture2D::Bind() const noexcept
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void GLTexture2D::ChangeFilterMode(Filter f) noexcept
	{
		constexpr GLint filterIDs[] = { GL_NEAREST, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR };

		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterIDs[static_cast<int>(f)]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterIDs[static_cast<int>(f)]);

		Texture2D::ChangeFilterMode(f);
	}

	void GLTexture2D::ChangeWrapMode(Wrap w) noexcept
	{
		constexpr GLint wrapIDs[] = { GL_REPEAT, GL_CLAMP_TO_EDGE };

		Bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapIDs[static_cast<int>(w)]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapIDs[static_cast<int>(w)]);

		Texture2D::ChangeWrapMode(w);
	}

	void GLTexture2D::GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp)
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

	uint32_t GLTexture2D::GetID() const noexcept
	{
		return m_TextureID;
	}
}
