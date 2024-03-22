#pragma once

#include <Engine.h>

class FlappyBirdApp : public Engine::App
{
public:
	FlappyBirdApp();
	~FlappyBirdApp();
	virtual void Start();
	virtual void Update();

private:
	Engine::Shader* m_Shader;
	Engine::BatchRenderer* m_Renderer;
	Engine::Viewport* m_Viewport;
	Engine::Camera* m_Camera;
	Engine::Texture2D* m_BackgroundTexture;
	std::vector<Engine::Texture2D*>	m_CharacterTextures;
	float m_ElapsedTime = 0.0f;
	
	glm::vec3 m_BirdPosition{ 0.0f };
	glm::vec3 m_BirdVelocity{ 0.0f };
	float m_BirdRotationZ = 0.0f;
};
