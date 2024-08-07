#include "Graphics.h"
#include "Engine/App/Window.h"

#include "Engine/Platform/GL/GLShader.h"
#include "Engine/Platform/GL/GLTexture2D.h"
#include "Engine/Platform/GL/GLBatchRenderer.h"
#include "Engine/Platform/GL/GLUIRenderer.h"

#include "Engine/Platform/D3D11/DX11Viewport.h"
#include "Engine/Platform/D3D11/DX11Shader.h"

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
		default:
			// TODO: Throw exception
			break;
		}

		return nullptr;
	}

	Viewport* Graphics::CreateViewport(Window* targetWindow, int x, int y, int w, int h) const
	{
		switch (m_RendererAPI)
		{
		case Engine::Graphics::Api::D3D11: return new DX11Viewport(targetWindow, x, y, w, h);
		case Engine::Graphics::Api::OPENGL: return new Viewport(targetWindow, x, y, w, h);
		default:
			// TODO: Throw exception
			break;
		}

		return nullptr;
	}

	Shader* Graphics::CreateShader(const std::string& vsFile, const std::string& fsFile) const
	{
		switch (m_RendererAPI)
		{
		case Engine::Graphics::Api::D3D11: return new DX11Shader(vsFile, fsFile);
		case Engine::Graphics::Api::OPENGL: return new GLShader(vsFile, fsFile);
		default:
			// TODO: Throw exception
			break;
		}

		return nullptr;
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
		default:
			// TODO: Throw exception
			break;
		}

		return nullptr;
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
		default:
			// TODO: Throw exception
			break;
		}

		return nullptr;
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
		default:
			// TODO: Throw error
			break;
		}

		return nullptr;
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
		default:
			// TODO: Throw error
			break;
		}

		return nullptr;
	}

#pragma endregion
}
