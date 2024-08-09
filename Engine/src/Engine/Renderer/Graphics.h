#pragma once

#include "Viewport.h"
#include "Shader.h"
#include "Texture2D.h"
#include "BatchRenderer.h"
#include "UI/UIRenderer.h"
#include "Engine/Utils/EngineException.h"

namespace Engine
{
	class Window;

	class Graphics
	{
	public:
		enum class Api { D3D11, OPENGL, NONE };
	public:
		class ApiNotSetException : public EngineException
		{
		public:
			ApiNotSetException(int line, const char* file, Graphics* gfxInstance) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			Graphics* GetGraphicsObject() const noexcept;
		private:
			Graphics* m_Gfx;
		};
	public:
		Graphics(Window* targetWindow);
		Graphics(const Graphics&) = delete;
		virtual Graphics& operator=(const Graphics&) = delete;
		virtual ~Graphics();

		virtual void EndFrame();
		virtual void ClearBuffer(float red, float green, float blue, float alpha) noexcept;
		virtual void DrawIndexed(unsigned int count);

		Api GetAPI();

	public:
		// Factory methods
		Viewport* CreateViewport(Window* targetWindow) const;
		Viewport* CreateViewport(Window* targetWindow, int x, int y, int w, int h) const;
		Shader* CreateShader(const std::string& vsFile, const std::string& fsFile) const;
		Texture2D* CreateTexture2D(unsigned char* pixels, int width, int height, int channelCount, unsigned int pixelPerUnit = 100u, Texture2D::Filter filter = Texture2D::Filter::Bilinear, Texture2D::Wrap wrap = Texture2D::Wrap::Clamp) const;
		Texture2D* CreateTexture2D(const std::string& imageFile, unsigned int pixelPerUnit = 100u, bool forceRGBA = false, Texture2D::Filter filter = Texture2D::Filter::Bilinear, Texture2D::Wrap wrap = Texture2D::Wrap::Clamp);
		BatchRenderer* CreateBatchRenderer(Shader* shader) const;
		UIRenderer* CreateUIRenderer(Shader* textShader, Shader* imageShader) const;

	protected:
		Window* m_TargetWindow;
		Api m_RendererAPI;
	};
}
