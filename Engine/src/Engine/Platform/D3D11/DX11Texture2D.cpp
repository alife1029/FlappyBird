#include "engine_pch.h"
#include "DX11Texture2D.h"
#include "Engine/Utils/EngineException.h"
#include "Engine/App/AppManager.h"

namespace Engine
{
	DX11Texture2D::DX11Texture2D(unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit, Filter filter, Wrap wrap)
		: 
		Texture2D(pixels, width, height, channelCount, pixelPerUnit, filter, wrap)
	{
		m_Gfx = (DX11Graphics*)AppManager::GetRunningApplication()->GetWindow()->GetGfx();

		GenerateTextureFromBytes(pixels, width, height, channelCount, filter, wrap);
	}

	DX11Texture2D::DX11Texture2D(const std::string& imageFile, unsigned int pixelPerUnit, bool forceRGBA, Filter filter, Wrap wrap)
		:
		Texture2D(imageFile, pixelPerUnit, forceRGBA, filter, wrap)
	{
		m_Gfx = (DX11Graphics*)AppManager::GetRunningApplication()->GetWindow()->GetGfx();

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

	DX11Texture2D::~DX11Texture2D()
	{
		m_Sampler->Release();
		m_Texture->Release();
		m_TextureView->Release();
	}

	void DX11Texture2D::Bind() const noexcept
	{
		m_Gfx->GetContext()->PSSetShaderResources(0, 1, &m_TextureView);
		m_Gfx->GetContext()->PSSetSamplers(0, 1, &m_Sampler); // TODO: Multi-sampler array
	}

	void DX11Texture2D::ChangeFilterMode(Filter f) noexcept
	{
		// TODO: Check is this working properly
		constexpr D3D11_FILTER filterIDs[] = { D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT, D3D11_FILTER_MIN_MAG_MIP_LINEAR };

		D3D11_SAMPLER_DESC sd;
		m_Sampler->GetDesc(&sd);
		sd.Filter = filterIDs[static_cast<size_t>(f)];

		// TODO: Apply changes
	}
	
	void DX11Texture2D::ChangeWrapMode(Wrap w) noexcept
	{
		// TODO: Check is this working properly
		constexpr D3D11_TEXTURE_ADDRESS_MODE wrapIDs[] = { D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_CLAMP };

		D3D11_SAMPLER_DESC sd;
		m_Sampler->GetDesc(&sd);
		sd.AddressU = sd.AddressV = sd.AddressW = wrapIDs[static_cast<size_t>(w)];

		// TODO: Apply changes
	}
	
	void DX11Texture2D::GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp)
	{
		// TODO: Graphics error handling

		// Create texture resource
		D3D11_TEXTURE2D_DESC td = {};
		td.Width = w;
		td.Height = h;
		td.MipLevels = 1;	// TODO: Create different mip-map levels
		td.ArraySize = 1;
		td.Format = ch == 4 ? DXGI_FORMAT_R8G8B8A8_UNORM	// TODO: Make sure these formats are working properly
					: ch == 3 ? DXGI_FORMAT_B8G8R8X8_UNORM
					: ch == 2 ? DXGI_FORMAT_R8G8_UNORM
					: ch == 1 ? DXGI_FORMAT_R8_UNORM
					: DXGI_FORMAT_UNKNOWN; // TODO: Throw unsupported image fomat error
		td.SampleDesc.Count = 1; // TODO: Antialiasing
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = NULL;
		td.MiscFlags = NULL;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = pixels;
		sd.SysMemPitch = w * ch * sizeof(unsigned char);

		// Create texture
		m_Gfx->GetDevice()->CreateTexture2D(&td, &sd, &m_Texture);

		// Craete shader resource view on the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = td.Format;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MostDetailedMip = 0;
		srvd.Texture2D.MipLevels = 1;

		m_Gfx->GetDevice()->CreateShaderResourceView(m_Texture, &srvd, &m_TextureView);

		// TODO: Implement anisotropic filtering
		// Craete sampler
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = f == Filter::Bilinear ? D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT
			: f == Filter::Trilinear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR
			: D3D11_FILTER_MIN_MAG_MIP_POINT;
		const D3D11_TEXTURE_ADDRESS_MODE a = wrp == Wrap::Clamp ? D3D11_TEXTURE_ADDRESS_CLAMP : D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressU = a;
		samplerDesc.AddressV = a;
		samplerDesc.AddressW = a;

		m_Gfx->GetDevice()->CreateSamplerState(&samplerDesc, &m_Sampler);
	}
}
