#include "engine_pch.h"
#include "BatchRenderer.h"
#include "Stats.h"
#include "Engine/App/AppManager.h"
#include "Engine/Platform/GL/GLShader.h"
#include "Engine/Platform/GL/GLTexture2D.h"

#define DEFAULT_SCALE glm::vec3 { 1.0f, 1.0f, 0.0f }
#define DEFAULT_ROTATION 0.0f
#define DEFAULT_COLOR { 1.0f, 1.0f, 1.0f, 1.0f }

namespace Engine
{
	BatchRenderer::BatchRenderer(Shader* shader)
	{
		
	}

	BatchRenderer::~BatchRenderer()
	{
		
	}

	void BatchRenderer::Begin(const glm::mat4& viewProjection)
	{
		m_ViewProj = viewProjection;
	}

	void BatchRenderer::End()
	{
		++RendererStats::s_BatchCount;
	}

	void BatchRenderer::DrawTexture(Texture2D* texture, const glm::mat4& transform, const glm::vec4& color)
	{
		RendererStats::s_TriangleCount += 2;
		RendererStats::s_VertexCount += 4;
	}

	void BatchRenderer::DrawTexture(Texture2D* texture, const glm::vec3& pos, const glm::vec3& scale, float zRotation, const glm::vec4& color)
	{
		DrawTexture(
			texture,
			glm::scale(
				glm::rotate(
					glm::translate(
						glm::mat4(1.0f),
						pos
					),
					glm::radians(zRotation),
					{ 0.0f, 0.0f, 1.0f }
				),
				scale
			),
			color
		);
	}
	void BatchRenderer::DrawTexture(Texture2D* texture, const glm::vec3& pos, const glm::vec3& scale, const glm::vec4& color)
	{
		DrawTexture(texture, pos, scale, DEFAULT_ROTATION, color);
	}
	void BatchRenderer::DrawTexture(Texture2D* texture, const glm::vec3& pos, float rotation, const glm::vec4& color)
	{
		DrawTexture(texture, pos, DEFAULT_SCALE, rotation, color);
	}
	void BatchRenderer::DrawTexture(Texture2D* texture, const glm::vec3& pos, const glm::vec4& color)
	{
		DrawTexture(texture, pos, DEFAULT_SCALE, DEFAULT_ROTATION, color);
	}
}
