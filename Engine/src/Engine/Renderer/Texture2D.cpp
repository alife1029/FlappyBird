#include "engine_pch.h"
#include "Texture2D.h"
#include "Engine/Utils/EngineException.h"
#include "Engine/App/AppManager.h"
#include "Engine/Platform/GL/GLTexture2D.h"

namespace Engine
{
	Texture2D* Texture2D::LoadWhiteTexture()
	{
		unsigned char pixelData[] = { 255, 255, 255 };

		switch (AppManager::GetRunningApplication()->GetWindow()->GetGfx()->GetAPI())
		{
		case Graphics::Api::OPENGL:
			return new GLTexture2D(pixelData, 1, 1, 3, 1, Filter::Point, Wrap::Clamp);
		}

		return nullptr;
	}

	Texture2D::Texture2D(unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit, Filter filter, Wrap wrap)
		:
		m_Width(width), m_Height(height), m_ChannelCount(channelCount), m_PixelPerUnit(pixelPerUnit), m_Filter(filter), m_Wrap(wrap)
	{
		
	}

	Texture2D::Texture2D(const std::string& imageFile, unsigned int pixelPerUnit, bool forceRGBA, Filter filter, Wrap wrap)
		:
		m_Width(0), m_Height(0), m_ChannelCount(0), m_PixelPerUnit(pixelPerUnit), m_Filter(filter), m_Wrap(wrap)
	{
		
	}

	Texture2D::~Texture2D()
	{
	}

	void Texture2D::Bind() const noexcept
	{
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

	void Texture2D::SetPixelPerUnit(int ppu) noexcept
	{
		m_PixelPerUnit = ppu;
	}
	void Texture2D::ChangeFilterMode(Filter f) noexcept
	{	
		m_Filter = f;
	}
	void Texture2D::ChangeWrapMode(Wrap w) noexcept
	{
		m_Wrap = w;
	}

	void Texture2D::GenerateTextureFromBytes(unsigned char* pixels, int w, int h, int ch, Filter f, Wrap wrp)
	{
		
	}
}
