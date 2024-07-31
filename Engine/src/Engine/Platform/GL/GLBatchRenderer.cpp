#include "engine_pch.h"
#include "GLBatchRenderer.h"
#include "GLTexture2D.h"

namespace Engine
{
	static constexpr size_t	QUAD_PER_BATCH = 1000;
	extern int				MAX_TEXTURES; // TODO: Think about moving this to the Graphics

	GLBatchRenderer::GLBatchRenderer(Shader* shader) : BatchRenderer(shader)
	{
		m_ShaderProgram = (GLShader*)shader;
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

	GLBatchRenderer::~GLBatchRenderer()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);

		delete[] m_TextureSlots;
		delete[] m_QuadBuffer;
	}

	void GLBatchRenderer::Begin(const glm::mat4& viewProjection)
	{
		m_QuadbufferPtr = m_QuadBuffer;
		BatchRenderer::Begin(viewProjection);
	}

	void GLBatchRenderer::End()
	{
		// Load vertex data to the VRAM
		GLsizeiptr size = (uint8_t*)m_QuadbufferPtr - (uint8_t*)m_QuadBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, (const char*)m_QuadBuffer);

		// Activate shader pipeline
		m_ShaderProgram->Bind();

		// Bind texture layer
		int* samplers = new int[m_TextureSlotIndex];
		for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
		{
			glBindTextureUnit(i, m_TextureSlots[i]);
			samplers[i] = i;
		}

		
		m_ShaderProgram->SetUniformMatrix4("u_ViewProj", m_ViewProj);
		m_ShaderProgram->SetUniformIntArray("u_Samplers", samplers, m_TextureSlotIndex);

		// Bind
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_SHORT, (const void*)0);

		m_IndexCount = 0;
		m_TextureSlotIndex = 0;

		delete[] samplers;

		BatchRenderer::End();
	}

	void GLBatchRenderer::DrawTexture(Texture2D* texture, const glm::mat4& transform, const glm::vec4& color)
	{
		const GLTexture2D* tex = (GLTexture2D*)texture;

		if (m_IndexCount >= QUAD_PER_BATCH)
		{
			End();
			Begin(m_ViewProj);
		}

		// Is the texture has used in the current batch?
		float texIndex = -1.0f;
		for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
		{
			if (m_TextureSlots[i] == tex->GetID())
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
				Begin(m_ViewProj);
			}

			texIndex = static_cast<float>(m_TextureSlotIndex);
			m_TextureSlots[m_TextureSlotIndex] = tex->GetID();
			++m_TextureSlotIndex;
		}

		// Initial vertex data
		float right = ((float)tex->GetWidth() / tex->PixelPerUnit()) / 2.0f;
		float top = ((float)tex->GetHeight() / tex->PixelPerUnit()) / 2.0f;
		glm::vec4 vertexPoses[] = {
			{ -right, -top, 0.0f, 1.0f },
			{  right, -top, 0.0f, 1.0f },
			{  right,  top, 0.0f, 1.0f },
			{ -right,  top, 0.0f, 1.0f }
		};
		glm::vec2 texCoords[] = {
			{ 0.0f, 0.0f },
			{ 1.0f, 0.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 1.0f }
		};

		// Calculate each vertex
		for (size_t i = 0; i < 4; i++)
		{
			m_QuadbufferPtr->Position = transform * vertexPoses[i];
			m_QuadbufferPtr->Color = color;
			m_QuadbufferPtr->TextureCoords = texCoords[i];
			m_QuadbufferPtr->TextureIndex = texIndex;
			++m_QuadbufferPtr;
		}

		m_IndexCount += 6;

		BatchRenderer::DrawTexture(texture, transform, color);
	}
}
