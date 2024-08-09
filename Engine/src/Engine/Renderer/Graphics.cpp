#include "Graphics.h"
#include "Engine/App/Window.h"

#include "Engine/Platform/GL/GLShader.h"
#include "Engine/Platform/GL/GLTexture2D.h"
#include "Engine/Platform/GL/GLBatchRenderer.h"
#include "Engine/Platform/GL/GLUIRenderer.h"

#include "Engine/Platform/D3D11/DX11Viewport.h"
#include "Engine/Platform/D3D11/DX11Shader.h"

#define THROW_API_NOT_SET() throw ApiNotSetException(__LINE__, __FILE__, m_TargetWindow->GetGfx())

namespace Engine
{
	Graphics::Graphics(Window* targetWindow)
		: m_TargetWindow(targetWindow), m_RendererAPI(Api::NONE)
	{
	}

	Graphics::~Graphics()
	{
	}

	void Graphics::EndFrame()
	{
	}

	void Graphics::ClearBuffer(float red, float green, float blue, float alpha) noexcept
	{
	}

	void Graphics::DrawIndexed(unsigned int count)
	{
	}

	Graphics::Api Graphics::GetAPI()
	{
		return m_RendererAPI;
	}

#pragma region Factory functions
	
	Viewport* Graphics::CreateViewport(Window* targetWindow) const
	{
		switch (m_RendererAPI)
		{
		case Engine::Graphics::Api::D3D11: return new DX11Viewport(targetWindow);
		case Engine::Graphics::Api::OPENGL: return new Viewport(targetWindow);
		default: THROW_API_NOT_SET();
		}
	}

	Viewport* Graphics::CreateViewport(Window* targetWindow, int x, int y, int w, int h) const
	{
		switch (m_RendererAPI)
		{
		case Engine::Graphics::Api::D3D11: return new DX11Viewport(targetWindow, x, y, w, h);
		case Engine::Graphics::Api::OPENGL: return new Viewport(targetWindow, x, y, w, h);
		default: THROW_API_NOT_SET();
		}
	}

	Shader* Graphics::CreateShader(const std::string& vsFile, const std::string& fsFile) const
	{
		switch (m_RendererAPI)
		{
		case Engine::Graphics::Api::D3D11: return new DX11Shader(vsFile, fsFile);
		case Engine::Graphics::Api::OPENGL: return new GLShader(vsFile, fsFile);
		default: THROW_API_NOT_SET();
		}
	}

	Texture2D* Graphics::CreateTexture2D(unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit, Texture2D::Filter filter, Texture2D::Wrap wrap) const
	{
		switch (m_RendererAPI)
		{
		case Engine::Graphics::Api::D3D11:
			// TODO: Create D3D11 Texture 2D
			break;
		case Engine::Graphics::Api::OPENGL:
			return new GLTexture2D(pixels, width, height, channelCount, pixelPerUnit, filter, wrap);
		default: THROW_API_NOT_SET();
		}
	}

	Texture2D* Graphics::CreateTexture2D(const std::string& imageFile, unsigned int pixelPerUnit, bool forceRGBA, Texture2D::Filter filter, Texture2D::Wrap wrap)
	{
		switch (m_RendererAPI)
		{
		case Engine::Graphics::Api::D3D11:
			// TODO: Create D3D11 Texture 2D
			break;
		case Engine::Graphics::Api::OPENGL:
			return new GLTexture2D(imageFile, pixelPerUnit, forceRGBA, filter, wrap);
			break;
		default: THROW_API_NOT_SET();
		}
	}

	BatchRenderer* Graphics::CreateBatchRenderer(Shader* shader) const
	{
		switch (m_RendererAPI)
		{
		case Engine::Graphics::Api::D3D11:
			// TODO: Create D3D11 Batch Renderer
			break;
		case Engine::Graphics::Api::OPENGL:
			return new GLBatchRenderer(shader);
		default: THROW_API_NOT_SET();
		}
	}

	UIRenderer* Graphics::CreateUIRenderer(Shader* textShader, Shader* imageShader) const
	{
		switch (m_RendererAPI)
		{
		case Engine::Graphics::Api::D3D11:
			// TODO: Create D3D11 UI Renderer
			break;
		case Engine::Graphics::Api::OPENGL:
			return new GLUIRenderer(textShader, imageShader);
		default: THROW_API_NOT_SET();
		}
	}

#pragma endregion

#pragma region Exceptions

	Graphics::ApiNotSetException::ApiNotSetException(int line, const char* file, Graphics* gfxInstance) noexcept
		:
		EngineException(line, file), m_Gfx(gfxInstance)
	{
	}

	const char* Graphics::ApiNotSetException::what() const noexcept
	{
		Api api = m_Gfx->GetAPI();
		std::string apistr = api == Api::D3D11 ? "D3D11" : api == Api::OPENGL ? "OpenGL" : api == Api::NONE ? "None" : "NULL";

		std::ostringstream oss;
		oss << GetType() << std::endl << std::endl
			<< "[Graphics Object]" << std::endl
			<< "	[Selected API] " << apistr << std::endl
			<< "	[Target Window]" << std::endl
			<< "		[Title] " << m_Gfx->m_TargetWindow->GetTitle() << std::endl
			<< "		[Dimension] " << m_Gfx->m_TargetWindow->GetWidth() << "x" << m_Gfx->m_TargetWindow->GetHeight() << std::endl << std::endl
			<< GetOriginString();

		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	const char* Graphics::ApiNotSetException::GetType() const noexcept
	{
		return "Renderer API Not Set";
	}

	Graphics* Graphics::ApiNotSetException::GetGraphicsObject() const noexcept
	{
		return m_Gfx;
	}

#pragma endregion
}
