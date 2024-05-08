#include "Bird.h"

using namespace Engine;

Bird::Bird(Texture2D** textures, unsigned int wingSound)
	:
	m_Textures(textures), m_CurrentTexture(textures[0]), Position({0.0f}), 
	m_Rotation(0.0f), m_Velocity({0.0f}), m_WingSound(wingSound)
{
	m_AudioSource = new AudioSource();
}

void Bird::Start()
{
	Position.y = 0.0f;
	m_Velocity.y = 0.0f;
}

void Bird::Update(float timeElapsed)
{
	// Set the current texture of the animation
	m_CurrentTexture = m_Textures[static_cast<size_t>(timeElapsed * 8.0f) % 3];

	Position += m_Velocity * Time::Delta();
	m_Velocity.y -= 9.807f * Time::Delta();

	if (Input::IsKeyJustPressed(Key::Space))
	{
		m_Velocity.y = 3.35f;
		m_AudioSource->Play(m_WingSound);
	}

	m_Rotation = 45.0f * m_Velocity.y / 10.0f;
}

void Bird::Render(Engine::BatchRenderer* renderer)
{
	renderer->DrawTexture(m_CurrentTexture, Position, m_Rotation);
}
