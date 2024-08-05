#pragma once

#include "Font.h"
#include "../Shader.h"
#include "../Vertex.h"

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Engine
{
	enum class Anchor
	{
		TopLeft = 0, TopCenter = 1, TopRight = 2,
		MiddleLeft = 3, MiddleCenter = 4, MiddleRight = 5,
		BottomLeft = 6, BottomCenter = 7, BottomRight = 8
	};

	class UIRenderer
	{
	public:
		UIRenderer(Shader* textShader, Shader* imageShader);
		virtual ~UIRenderer();

		void Begin(const glm::ivec2& windowDimensions);
		void End();

		void DrawTxt(Font* fontFamily, const std::string& text, const glm::vec2& position, float fontSize = 12.0f, const glm::vec4& color = { 0.05f, 0.05f, 0.05f, 1.0f });
		void DrawTxt(Font* fontFamily, const std::string& text, const glm::vec2& position, Anchor anchor, float fontSize = 12.0f, const glm::vec4& color = { 0.05f, 0.05f, 0.05f, 1.0f });
		virtual void DrawChar(Font* fontFamily, char chr, const glm::vec2& position, float fontSize = 12.0f, const glm::vec4& color = { 0.05f, 0.05f, 0.05f, 1.0f });
		virtual void DrawImage(Texture2D* img, const glm::vec2& position, const glm::vec2& scale, float rotation = 0.0f, Anchor anchor = Anchor::MiddleCenter, Anchor pivot = Anchor::MiddleCenter, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	protected:
		virtual void InitializeTextRenderer(Shader* textShader);
		virtual void InitializeImageRenderer(Shader* imageShader);
		virtual void DestroyTextRenderer();
		virtual void DestroyImageRenderer();

		virtual void BeginTextRenderer();
		virtual void BeginImageRenderer();
		virtual void EndTextRenderer();
		virtual void EndImageRenderer();

	protected:
		Shader		*m_TextShader			= nullptr,
					*m_ImageShader			= nullptr;
		glm::ivec2	m_WindowDimensions		= glm::ivec2{ 0 };
	};
}
