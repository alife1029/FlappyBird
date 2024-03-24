#pragma once

#include "Engine.h"

class Background
{
public:
	Background(Engine::Texture2D* tex, const glm::vec3& vel);
	void Update();
	void Render(Engine::BatchRenderer* renderer);

private:
	Engine::Texture2D* m_Texture;
	glm::vec3 m_Pos;
	glm::vec3 m_Velocity;
};
