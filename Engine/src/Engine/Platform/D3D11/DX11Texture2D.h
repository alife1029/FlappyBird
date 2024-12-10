#pragma once

#include "Engine/Renderer/Texture2D.h"
#include "DX11Graphics.h"

namespace Engine
{
	class DX11Texture2D : public Texture2D
	{
	public:
		DX11Texture2D(unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit = 100u, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);
		DX11Texture2D(const std::string& imageFile, unsigned int pixelPerUnit = 100u, bool forceRGBA = false, Filter filter = Filter::Bilinear, Wrap wrap = Wrap::Clamp);
		~DX11Texture2D();

		void Bind() const noexcept override;

		void ChangeFilterMode(Filter f) noexcept override;
		void ChangeWrapMode(Wrap w) noexcept override;

	protected:
		void GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp) override;

	private:
		DX11Graphics* m_Gfx;
		ID3D11Texture2D* m_Texture;
		ID3D11ShaderResourceView* m_TextureView;
		ID3D11SamplerState* m_Sampler;
	};
}
