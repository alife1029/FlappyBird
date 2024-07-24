#pragma once

#include "Engine/Renderer/Texture2D.h"

namespace Engine
{
	class GLTexture2D : public Texture2D
	{
	public:
		GLTexture2D(unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit = 100u, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);
		GLTexture2D(const std::string& imageFile, unsigned int pixelPerUnit = 100u, bool forceRGBA = false, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);
		~GLTexture2D();

		void Bind() const noexcept override;

		void ChangeFilterMode(Filter f) noexcept override;
		void ChangeWrapMode(Wrap w) noexcept override;

		uint32_t GetID() const noexcept;

	protected:
		void GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp) override;

	private:
		uint32_t	m_TextureID = 0;
	};
}
