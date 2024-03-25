#pragma once

#include "Engine.h"

class Pipe
{
public:
	Pipe(Engine::Texture2D* texture, float positionX, const glm::vec3& velocity);

	void Update();
	void Render(Engine::BatchRenderer* renderer);

public:
	glm::vec3 Position;

private:
	Engine::Texture2D* m_Texture;
	glm::vec3 m_Velocity;
	const float m_Gap = 1.0f;
};
