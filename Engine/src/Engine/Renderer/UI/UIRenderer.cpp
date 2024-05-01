#include "engine_pch.h"
#include "UIRenderer.h"
#include "Font.h"
#include "../Stats.h"
#include "Engine/Utils/EngineException.h"

namespace Engine
{
	extern int				MAX_TEXTURES;
	static constexpr size_t	QUAD_PER_BATCH = 1000;

	UIRenderer::UIRenderer(Shader* shader)
	{
		m_ShaderProgram = shader;
		m_QuadBuffer = new Vertex[QUAD_PER_BATCH];

		// Calculate indices
		const size_t indexCount = QUAD_PER_BATCH * 6;
		GLushort* indices = new GLushort[indexCount];
		uint16_t offset = 0;
		for (size_t i = 0; i < indexCount; i += 6)
		{
			indices[i] = offset;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset;
			indices[i + 4] = offset + 2;
			indices[i + 5] = offset + 3;
			offset += 4;
		}

		// Generate OpenGL buffers
		glCreateVertexArrays(1, &m_VAO);
		glCreateBuffers(1, &m_VBO);
		glCreateBuffers(1, &m_EBO);

		// Bind OpenGL buffers
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

		// Load OpenGL buffer datas
		glBufferData(GL_ARRAY_BUFFER, QUAD_PER_BATCH * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLushort), indices, GL_STATIC_DRAW);

		// Vertex attributes
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TextureCoords));
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TextureIndex));

		// Unbind buffers
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Delete indices array
		delete[] indices;

		// Texture slots initialization
		m_TextureSlots = new uint32_t[MAX_TEXTURES];
		for (size_t i = 0; i < MAX_TEXTURES; i++)
			m_TextureSlots[i] = 0;
		m_TextureSlotIndex = 0;
	}

	UIRenderer::~UIRenderer()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);

		delete[] m_TextureSlots;
		delete[] m_QuadBuffer;
	}

	void UIRenderer::Begin(const glm::ivec2& windowDimension)
	{
		m_QuadbufferPtr = m_QuadBuffer;
		m_WindowDimensions = windowDimension;
	}

	void UIRenderer::End()
	{
		// Load vertex data to the GPU
		GLsizeiptr size = (uint8_t*)m_QuadbufferPtr - (uint8_t*)m_QuadBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, (const void*)m_QuadBuffer);

		// Activate shader pipeline
		m_ShaderProgram->Bind();

		// Bind texture layer
		int* samplers = new int[m_TextureSlotIndex];
		for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
		{
			glBindTextureUnit(i, m_TextureSlots[i]);
			samplers[i] = i;
		}

		// Update uniform variables
		m_ShaderProgram->SetUniformVec2("u_WindowSize", { static_cast<float>(m_WindowDimensions.x), static_cast<float>(m_WindowDimensions.y) });
		m_ShaderProgram->SetUniformIntArray("u_Samplers", samplers, m_TextureSlotIndex);

		// Binding
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

		// Drawing
		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_SHORT, (const void*)0);

		m_IndexCount = 0;
		m_TextureSlotIndex = 0;

		delete[] samplers;

		++RendererStats::s_BatchCount;
	}

	void UIRenderer::DrawImage(Texture2D* img, const glm::vec2& position, const glm::vec2& scale, float rotation, Anchor anchor, Anchor pivot, const glm::vec4& color)
	{
		if (m_IndexCount >= QUAD_PER_BATCH * 6)
		{
			End();
			Begin(m_WindowDimensions);
		}

		// Is the texture has used in the current batch?
		float texIndex = -1.0f;
		for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
		{
			if (m_TextureSlots[i] == img->GetID())
			{
				texIndex = static_cast<float>(i);
				break;
			}
		}

		// If not used, add the id of this texture to the texture slots array
		if (texIndex == -1.0f)
		{
			if (m_TextureSlotIndex >= static_cast<uint32_t>(MAX_TEXTURES) - 1)
			{
				End();
				Begin(m_WindowDimensions);
			}

			texIndex = static_cast<float>(m_TextureSlotIndex);
			m_TextureSlots[m_TextureSlotIndex] = img->GetID();
			++m_TextureSlotIndex;
		}

		glm::vec2 pos{ 0.0f };

		// Set position by anchor
		// x-axis
		switch (static_cast<int>(anchor) % 3)
		{
		case 0:	pos.x = position.x; break;									// LEFT
		case 1:	pos.x = m_WindowDimensions.x / 2.0f + position.x; break;	// CENTER
		case 2:	pos.x = m_WindowDimensions.x - position.x; break;			// RIGHT
		}
		
		// y-axis
		switch (static_cast<int>(anchor) / 3)
		{
		case 0: pos.y = m_WindowDimensions.y - position.y; break;			// TOP
		case 1:	pos.y = m_WindowDimensions.y / 2.0f + position.y; break;	// MIDDLE
		case 2: pos.y = position.y; break;									// BOTTOM
		}

		// Set vertices by pivot
		glm::vec4 vertexPoses[] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f, 1.0f },
			{ 1.0f, 1.0f, 0.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
		};

		glm::vec2 texCoords[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f },
		};

		// Pivot
		for (glm::vec4& vert : vertexPoses)
		{
			vert -= glm::vec4{ 
				0.5f * (static_cast<int>(pivot) % 3),
				0.5f * (2 - static_cast<int>(pivot) / 3),
				0.0f,
				0.0f,
			};
		}

		// Transform vertices
		for (size_t i = 0; i < 4; i++)
		{
			m_QuadbufferPtr->Position = glm::scale(
				glm::rotate(
					glm::translate(
						glm::mat4(1.0f),
						{ pos.x, pos.y, 0.0f}
					),
					glm::radians(rotation),
					{ 0.0f, 0.0f, 1.0f }
				),
				{ scale.x, scale.y, 0.0f }
			) * vertexPoses[i];
			m_QuadbufferPtr->Color = color;
			m_QuadbufferPtr->TextureCoords = texCoords[i];
			m_QuadbufferPtr->TextureIndex = texIndex;
			++m_QuadbufferPtr;
		}

		m_IndexCount += 6;

		RendererStats::s_TriangleCount += 2;
		RendererStats::s_VertexCount += 4;
	}

	void UIRenderer::DrawTxt(Font* fontFamily, const std::string& text, const glm::vec2& position, float fontSize, const glm::vec4& color)
	{
		glm::vec2 offset{ 0.0f };

		for (auto& c : text)
		{
			Font::Character ch = fontFamily->m_Characters[c];
			offset.x += ch.Bearing.x * fontSize / (float)fontFamily->m_FontSize;
			offset.y = -(ch.GlyphSize.y - ch.Bearing.y) * fontSize / (float)fontFamily->m_FontSize;
			DrawChar(fontFamily, c, position + offset, fontSize, color);
			offset.x += (ch.Advance >> 6) * fontSize / (float)fontFamily->m_FontSize;
		}
	}

	void UIRenderer::DrawTxt(Font* fontFamily, const std::string& text, const glm::vec2& position, Anchor anchor, float fontSize, const glm::vec4& color)
	{
		const float scaleFactor = fontSize / (float)fontFamily->m_FontSize;

		glm::vec2 textboxDimensions{ 0.0f };
		glm::vec2 textboxPosition;

		textboxDimensions.y = fontFamily->m_FontSize * scaleFactor;

		for (auto& c : text)
		{
			const Font::Character ch = fontFamily->m_Characters[c];
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
		if (m_IndexCount >= QUAD_PER_BATCH * 6)
		{
			End();
			Begin(m_WindowDimensions);
		}

		Font::Character c = fontFamily->m_Characters[chr];

		// Is the texture has used in the current batch?
		float texIndex = -1.0f;
		for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
		{
			if (m_TextureSlots[i] == c.TextureID)
			{
				texIndex = static_cast<float>(i);
				break;
			}
		}

		// If not used, add the id of this texture to the texture slots array
		if (texIndex == -1.0f)
		{
			if (m_TextureSlotIndex >= static_cast<uint32_t>(MAX_TEXTURES) - 1)
			{
				End();
				Begin(m_WindowDimensions);
			}

			texIndex = static_cast<float>(m_TextureSlotIndex);
			m_TextureSlots[m_TextureSlotIndex] = c.TextureID;
			++m_TextureSlotIndex;
		}

		// Initial vertex data
		float width = ((float)c.GlyphSize.x * fontSize / fontFamily->m_FontSize);
		float height = ((float)c.GlyphSize.y * fontSize / fontFamily->m_FontSize);

		glm::vec4 vertexPoses[] = {
			{ 0.0f, 0.0f, 0.0f, 1.0f },		// BOTTOM-LEFT
			{ width, 0.0f, 0.0f, 1.0f },	// BOTTOM-RIGHT
			{ width, height, 0.0f, 1.0f },	// TOP-RIGHT
			{ 0.0f,  height, 0.0f, 1.0f },	// TOP-LEFT
		};
		glm::vec2 texCoords[] = {
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 1.0f, 0.0f },
			{ 0.0f, 0.0f }
		};

		// Transform vertices
		for (size_t i = 0; i < 4; i++)
		{
			m_QuadbufferPtr->Position = glm::translate(glm::mat4(1.0f), {(float)position.x, (float)position.y, 0.0f}) * vertexPoses[i];
			m_QuadbufferPtr->Color = color;
			m_QuadbufferPtr->TextureCoords = texCoords[i];
			m_QuadbufferPtr->TextureIndex = texIndex;
			++m_QuadbufferPtr;
		}

		m_IndexCount += 6;

		RendererStats::s_TriangleCount += 2;
		RendererStats::s_VertexCount += 4;
	}
}
