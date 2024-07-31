#pragma once

#include "Texture2D.h"
#include "Shader.h"
#include "Vertex.h"

#include <glm/glm.hpp>

namespace Engine
{
	class BatchRenderer
	{
	public:
		BatchRenderer(Shader* shader);
		virtual ~BatchRenderer();

		virtual void Begin(const glm::mat4& viewProjection = glm::mat4(1.0f));
		virtual void End();

		virtual void DrawTexture(Texture2D* texture, const glm::mat4& transform, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTexture(Texture2D* texture, const glm::vec3& pos, const glm::vec3& scale, float zRotation, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTexture(Texture2D* texture, const glm::vec3& pos, const glm::vec3& scale, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTexture(Texture2D* texture, const glm::vec3& pos, float rotation, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		void DrawTexture(Texture2D* texture, const glm::vec3& pos, const glm::vec4& color = glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });

	protected:
		glm::mat4	m_ViewProj{ 1.0f };
	};
}
