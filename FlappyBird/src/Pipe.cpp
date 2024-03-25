#include "Pipe.h"
#include <cstdlib>

using namespace Engine;
using namespace glm;

Pipe::Pipe(Engine::Texture2D* texture, float positionX, const glm::vec3& velocity, Bird* bird)
	:
	m_Texture(texture), m_Velocity(velocity), m_Bird(bird)
{
	Position = { positionX, (std::rand() % 35 - 17) / 20.0f, 0.0f };
}

void Pipe::Update()
{
	Position += m_Velocity * Time::Delta();

	// Is it in the same x position with the bird
	if (std::abs(Position.x - m_Bird->Position.x) < (m_Texture->GetWidth() / 2.0f + 17.0f) / m_Texture->PixelPerUnit())
	{
		// Does the bird between the gap, or is it collides
		if (std::abs(Position.y - m_Bird->Position.y) > m_Gap / 2.0f - 0.12f)
		{
			Position.y += 10.0f;
		}
	}

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
