#include "engine_pch.h"
#include "TextRenderer.h"
#include "Font.h"
#include "Engine/Utils/EngineException.h"

namespace Engine
{
	extern int				MAX_TEXTURES;
	static constexpr size_t	QUAD_PER_BATCH = 1000;

	TextRenderer::TextRenderer(Shader* shader)
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

	TextRenderer::~TextRenderer()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);

		delete[] m_TextureSlots;
		delete[] m_QuadBuffer;
	}

	void TextRenderer::Begin(const glm::ivec2& windowDimension)
	{
		m_QuadbufferPtr = m_QuadBuffer;
		m_WindowDimensions = windowDimension;
	}

	void TextRenderer::End()
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
	}

	void TextRenderer::Draw(Font* fontFamily, const std::string& text, const glm::vec2& position, float fontSize, const glm::vec4& color)
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

	void TextRenderer::DrawChar(Font* fontFamily, char chr, const glm::vec2& position, float fontSize, const glm::vec4& color)
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
	}
}
