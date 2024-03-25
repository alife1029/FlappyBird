#include "Pipe.h"
#include <cstdlib>

using namespace Engine;
using namespace glm;

Pipe::Pipe(Texture2D* texture, float posX, const vec3& velocity)
	:
	m_Texture(texture), m_Velocity(velocity)
{
	Position = { posX, (std::rand() % 35 - 17) / 20.0f, 0.0f }; 
}

void Pipe::Update()
{
	Position += m_Velocity * Time::Delta();

	if (Position.x <= -9.0f)
	{
		Position.x += 18.0f;
		Position.y = (std::rand() % 35 - 17) / 20.0f;
	}
}

void Pipe::Render(Engine::BatchRenderer* renderer)
{
	// Upper pipe
	renderer->DrawTexture(
		m_Texture,
		Position + vec3{ 0.0f, m_Gap / 2.0f + ((float)m_Texture->GetHeight() / m_Texture->PixelPerUnit()) / 2.0f, 0.0f },
		{ 1.0f, -1.0f, 0.0f }
	);

	// Lower pipe
	renderer->DrawTexture(
		m_Texture,
		Position - vec3{ 0.0f, m_Gap / 2.0f + ((float)m_Texture->GetHeight() / m_Texture->PixelPerUnit()) / 2.0f, 0.0f },
		{ 1.0f, 1.0f, 0.0f }
	);
}
