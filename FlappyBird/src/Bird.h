#pragma once

#include "Engine.h"

class Bird
{
public:
	Bird(Engine::Texture2D** textures);
	~Bird();

	void Start();
	void Update(float timeElapsed);
	void Render(Engine::BatchRenderer* renderer);
	
public:
	glm::vec3 Position;

private:
	Engine::Texture2D** m_Textures;
	Engine::Texture2D* m_CurrentTexture;
	glm::vec3 m_Velocity;
	float m_Rotation;

	Engine::AudioSource* m_AudioSource;
	Engine::AudioClip* m_WingSfx;
};
