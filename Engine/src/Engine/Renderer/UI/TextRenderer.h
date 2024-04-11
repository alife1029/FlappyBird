#pragma once

#include "Font.h"
#include "../Shader.h"
#include "../Vertex.h"

#include <string>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

namespace Engine
{
	class TextRenderer
	{
	public:
		TextRenderer(Shader* shader);
		~TextRenderer();

		void Begin(const glm::ivec2& windowDimensions);
		void End();

		void Draw(Font* fontFamily, const std::string& text, const glm::vec2& position, float fontSize = 12.0f, const glm::vec4& color = { 0.05f, 0.05f, 0.05f, 1.0f });
		void DrawChar(Font* fontFamily, char chr, const glm::vec2& position, float fontSize = 12.0f, const glm::vec4& color = { 0.05f, 0.05f, 0.05f, 1.0f });

	private:
		uint32_t	m_VBO = 0,
					m_VAO = 0,
					m_EBO = 0,
					m_IndexCount = 0;
		Vertex		*m_QuadBuffer = nullptr,
					*m_QuadbufferPtr = nullptr;
		uint32_t	*m_TextureSlots = nullptr,
					m_TextureSlotIndex = 0;
		Shader*		m_ShaderProgram = nullptr;
		glm::ivec2	m_WindowDimensions = glm::ivec2{ 0 };
	};
}
