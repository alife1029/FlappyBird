#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera(const Viewport* viewport)
{
	m_Viewport = viewport;
	Update();
}

void OrthographicCamera::Update() noexcept
{
	glm::mat4 viewMatrix = glm::translate(
		glm::rotate(glm::mat4(1.0f), -glm::radians(m_Rotation), { 0.0f, 0.0f, 1.0f }),
		-m_Position
	);

	float left = -m_Size * m_Viewport->GetAspectRatio() / 2.0f;
	float top = m_Size / 2.0f;
	glm::mat4 projectionMatrix = glm::ortho(left, -left, -top, top);

	m_ViewProjMatrix = projectionMatrix * viewMatrix;
}
