#pragma once

#include "Engine.h"
#include "Bird.h"

#include <functional>

class Pipe
{
public:
	Pipe(Engine::Texture2D* texture, float positionX, const glm::vec3& velocity, Bird* bird);

	void Update(std::function<void(int)> onBetweenGaps);
	void Render(Engine::BatchRenderer* renderer);
	
public:
	glm::vec3 Position; 
	glm::vec3 Velocity;

private:
	Engine::Texture2D* m_Texture;
	const float m_Gap = 1.15f;
	Bird* m_Bird;
};
