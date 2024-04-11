#pragma once

#include <Engine.h>

#include "Background.h"
#include "Bird.h"
#include "Pipe.h"

class FlappyBirdApp : public Engine::App
{
public:
	FlappyBirdApp();
	~FlappyBirdApp();
	virtual void Start();
	virtual void Update();

private:
	Engine::Shader			*m_Shader,
							*m_TextShader;
	Engine::BatchRenderer*	m_Renderer;
	Engine::Viewport*		m_Viewport;
	Engine::Camera*			m_Camera;
	Engine::Texture2D		*m_BackgroundTexture,
							*m_PipeTexture;
	Engine::Font			*m_PixelifySans,
							*m_Righteous;
	Engine::TextRenderer*	m_TextRenderer;
	std::vector<Engine::Texture2D*>	m_CharacterTextures;
	float m_ElapsedTime = 0.0f;

	Background* m_Bg;
	Bird* m_Bird;
	std::vector<Pipe*> m_Pipes;

	float m_FpsHistory[500]{ 0 };
};
