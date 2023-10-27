#include "Texture2D.h"
#include "../Utils/File.h"

#include <glad/glad.h>
#include <stb/stb_image.h>

Texture2D::Texture2D()
{
	m_Width = 1;
	m_Height = 1;
	m_ChannelCount = 4;

	uint32_t pixels = 0xffffffff;

	GenerateObjectFromPixels(reinterpret_cast<unsigned char*>(&pixels));
}

Texture2D::Texture2D(const std::string& imageFile)
{
	stbi_set_flip_vertically_on_load(1);
	unsigned char* pixels = stbi_load(imageFile.c_str(), &m_Width, &m_Height, &m_ChannelCount, STBI_rgb_alpha);
	
	if (pixels)
	{
		GenerateObjectFromPixels(pixels);
	}
	else
	{
		throw File::NotFoundException(imageFile);
	}

	stbi_image_free(pixels);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture2D::Activate() const noexcept
{
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture2D::SetFilter(Filter filter) noexcept
{
	Activate();

	switch (filter)
	{
	case Texture2D::Filter::None:
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case Texture2D::Filter::Linear:
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case Texture2D::Filter::Trilinear:
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	}

	m_Filter = filter;
}

void Texture2D::SetWrap(Wrap wrap) noexcept
{
	Activate();

	switch (wrap)
	{
	case Texture2D::Wrap::Repeat:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case Texture2D::Wrap::MirroredRepeat:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case Texture2D::Wrap::ClampToEdge:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case Texture2D::Wrap::ClampToBorder:
		float borderColor[] = { 0.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		break;
	}

	m_Wrap = wrap;
}

void Texture2D::GenerateObjectFromPixels(unsigned char* pixels)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	Activate();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, NULL, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	SetFilter(Filter::None);
	SetWrap(Wrap::ClampToEdge);
	glGenerateMipmap(GL_TEXTURE_2D);
}
