#include "engine_pch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	Camera::Camera(Viewport* viewport)
	{
		SetViewport(viewport);
		Update();
	}

	void Camera::Update() noexcept
	{
		m_View = glm::translate(glm::rotate(glm::mat4(1.0f), -glm::radians(Rotation), { 0.0f, 0.0f, 1.0f }), -Position);
		m_Projection = glm::ortho(	Size * m_Viewport->GetAspectRatio() / -2.0f, 
									Size * m_Viewport->GetAspectRatio() / 2.0f, 
									Size / -2.0f, 
									Size / 2.0f);
		m_Combined = m_Projection * m_View;
	}

	void Camera::SetViewport(Viewport* viewport)
	{
		m_Viewport = viewport;
	}

	const glm::mat4& Camera::GetViewMatrix() const noexcept
	{
		return m_View;
	}
	const glm::mat4& Camera::GetProjectionMatrix() const noexcept
	{
		return m_Projection;
	}
	const glm::mat4& Camera::GetCombinedMatrix() const noexcept
	{
		return m_Combined;
	}
}
