#pragma once

#include "Viewport.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class OrthographicCamera
{
public:
	OrthographicCamera(const Viewport* viewport);

	void Update() noexcept;

	void SetSize(float size) noexcept { m_Size = size; }
	void ZoomIn(float percentage) noexcept { m_Size *= (1.0f + percentage / 100.0f); }
	void ZoomOut(float percentage) noexcept { m_Size *= (1.0f - percentage / 100.0f); }
	void SetRotation(float rotation) noexcept { m_Rotation = rotation; }
	void Rotate(float amount) noexcept { m_Rotation += amount; }
	void SetPosition(const glm::vec3& pos) noexcept { m_Position = pos; }
	void Translate(const glm::vec3& delta) noexcept { m_Position += delta; }
	void SetViewport(const Viewport* viewport) noexcept { m_Viewport = viewport; }

	float GetSize() const noexcept { return m_Size; }
	float GetRotation() const noexcept { return m_Rotation; }
	float GetPositionX() const noexcept { return m_Position.x; }
	float GetPositionY() const noexcept { return m_Position.y; }
	glm::vec3 GetPosition() const noexcept { return m_Position; }
	glm::mat4 GetViewProjMatrix() const noexcept { return m_ViewProjMatrix; }
	const Viewport* GetViewport() const noexcept { return m_Viewport; }
private:
	float				m_Size = 5.0f,
						m_Rotation = 0.0f;
	glm::vec3			m_Position = glm::vec3{ 0.0f };
	glm::mat4			m_ViewProjMatrix = glm::mat4{ 1.0f };
	const Viewport*		m_Viewport;
};
