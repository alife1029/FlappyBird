#pragma once

#include "Engine/Renderer/BatchRenderer.h"
#include "GLShader.h"

namespace Engine
{
	class GLBatchRenderer : public BatchRenderer
	{
	public:
		GLBatchRenderer(Shader* shader);
		~GLBatchRenderer();

		void Begin(const glm::mat4& viewProjection = glm::mat4(1.0f)) override;
		void End() override;

		void DrawTexture(Texture2D* texture, const glm::mat4& transform, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }) override;

	private:
		uint32_t	m_VBO = 0,
					m_VAO = 0,
					m_EBO = 0,
					m_IndexCount = 0;
		Vertex*		m_QuadBuffer = nullptr,
					*m_QuadbufferPtr = nullptr;
		uint32_t*	m_TextureSlots = nullptr,
					m_TextureSlotIndex = 0;
		GLShader*	m_ShaderProgram = nullptr;
	};
}
