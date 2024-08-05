#include "engine_pch.h"
#include "UIRenderer.h"
#include "Font.h"
#include "../Stats.h"
#include "Engine/Utils/EngineException.h"
#include "Engine/App/AppManager.h"
#include "Engine/Platform/GL/GLShader.h"
#include "Engine/Platform/GL/GLTexture2D.h"

namespace Engine
{
	UIRenderer::UIRenderer(Shader* textShader, Shader* imageShader)
	{
	}

	UIRenderer::~UIRenderer()
	{
		
	}

	void UIRenderer::Begin(const glm::ivec2& windowDimension)
	{
		BeginTextRenderer();
		BeginImageRenderer();
		m_WindowDimensions = windowDimension;
	}

	void UIRenderer::End()
	{
		EndTextRenderer();
		EndImageRenderer();
	}

	void UIRenderer::DrawTxt(Font* fontFamily, const std::string& text, const glm::vec2& position, float fontSize, const glm::vec4& color)
	{
		glm::vec2 offset{ 0.0f };

		for (auto& c : text)
		{
			Font::Character ch = fontFamily->GetChar(c);
			offset.x += ch.Bearing.x * fontSize / (float)fontFamily->GetFontSize();
			offset.y = -(ch.GlyphSize.y - ch.Bearing.y) * fontSize / (float)fontFamily->GetFontSize();
			DrawChar(fontFamily, c, position + offset, fontSize, color);
			offset.x += (ch.Advance >> 6) * fontSize / (float)fontFamily->GetFontSize();
		}
	}

	void UIRenderer::DrawTxt(Font* fontFamily, const std::string& text, const glm::vec2& position, Anchor anchor, float fontSize, const glm::vec4& color)
	{
		const float scaleFactor = fontSize / (float)fontFamily->GetFontSize();

		glm::vec2 textboxDimensions{ 0.0f };
		glm::vec2 textboxPosition;

		textboxDimensions.y = fontFamily->GetFontSize() * scaleFactor;

		for (auto& c : text)
		{
			const Font::Character ch = fontFamily->GetChar(c);
			textboxDimensions.x += (ch.Bearing.x + (ch.Advance >> 6)) * scaleFactor;
		}

		switch (anchor)
		{
		case Engine::Anchor::TopLeft:
			textboxPosition = {
				position.x,
				m_WindowDimensions.y - position.y - textboxDimensions.y
			};
			break;
		case Engine::Anchor::TopCenter:
			textboxPosition = {
				m_WindowDimensions.x / 2.0f + position.x - textboxDimensions.x / 2.0f,
				m_WindowDimensions.y - position.y - textboxDimensions.y
			};
			break;
		case Engine::Anchor::TopRight:
			textboxPosition = {
				m_WindowDimensions.x - position.x - textboxDimensions.x,
				m_WindowDimensions.y - position.y - textboxDimensions.y
			};
			break;
		
		case Engine::Anchor::MiddleLeft:
			textboxPosition = {
				position.x,
				m_WindowDimensions.y / 2.0f + position.y - textboxDimensions.y / 2.0f
			};
			break;
		case Engine::Anchor::MiddleCenter:
			textboxPosition = {
				m_WindowDimensions.x / 2.0f + position.x - textboxDimensions.x / 2.0f,
				m_WindowDimensions.y / 2.0f + position.y - textboxDimensions.y / 2.0f
			};
			break;
		case Engine::Anchor::MiddleRight:
			textboxPosition = {
				m_WindowDimensions.x - position.x - textboxDimensions.x,
				m_WindowDimensions.y / 2.0f + position.y - textboxDimensions.y / 2.0f
			};
			break;
		
		case Engine::Anchor::BottomLeft:
			textboxPosition = {
				position.x,
				position.y
			};
			break;
		case Engine::Anchor::BottomCenter:
			textboxPosition = {
				m_WindowDimensions.x / 2.0f + position.x - textboxDimensions.x / 2.0f,
				position.y
			};
			break;
		case Engine::Anchor::BottomRight:
			textboxPosition = {
				m_WindowDimensions.x - position.x - textboxDimensions.x,
				position.y
			};
			break;
		}
	
		DrawTxt(fontFamily, text, textboxPosition, fontSize, color);
	}

	void UIRenderer::DrawChar(Font* fontFamily, char chr, const glm::vec2& position, float fontSize, const glm::vec4& color)
	{
		RendererStats::s_TriangleCount += 2;
		RendererStats::s_VertexCount += 4;
	}

	void UIRenderer::DrawImage(Texture2D* img, const glm::vec2& position, const glm::vec2& scale, float rotation, Anchor anchor, Anchor pivot, const glm::vec4& color)
	{
		RendererStats::s_TriangleCount += 2;
		RendererStats::s_VertexCount += 4;
	}

	void UIRenderer::InitializeTextRenderer(Shader* textShader) { }
	void UIRenderer::InitializeImageRenderer(Shader* imageShader) { }

	void UIRenderer::DestroyTextRenderer() { }
	void UIRenderer::DestroyImageRenderer() { }

	void UIRenderer::BeginTextRenderer() { }
	void UIRenderer::BeginImageRenderer() { }

	void UIRenderer::EndTextRenderer()
	{
		++RendererStats::s_BatchCount;
	}

	void UIRenderer::EndImageRenderer()
	{
		++RendererStats::s_BatchCount;
	}
}
