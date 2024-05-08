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
	void StartGame();
	void ResetGame();

	void RenderObjects();
	void RenderUI();
	void RenderImGui();

private:
	void OnBirdBetweenPipes(bool isCollide);

private:
	Engine::Shader			*m_Shader,
							*m_TextShader,
							*m_UiImageShader;
	Engine::BatchRenderer*	m_Renderer;
	Engine::Viewport*		m_Viewport;
	Engine::Camera*			m_Camera;
	Engine::Texture2D		*m_BackgroundTexture,
							*m_PipeTexture,
							*m_WhiteTexture;
	Engine::Font			*m_PixelifySans,
							*m_Righteous;
	Engine::UIRenderer*	m_UiRenderer;
	Engine::AudioDevice* m_AudioDevice = nullptr;
	Engine::AudioSource* m_AudioSource = nullptr;
	unsigned int m_ParadiseCity = 0;
	std::vector<Engine::Texture2D*>	m_CharacterTextures;
	float m_ElapsedTime = 0.0f;

	unsigned int	m_Score = 0,
					m_HiScore = 0;

	bool m_BirdBetweenGaps = false;
	bool m_BirdNearPipes = false;

	bool m_GameRunning = false;
	bool m_FirstLaunched = true;

	Background* m_Bg;
	Bird* m_Bird;
	std::vector<Pipe*> m_Pipes;

	float m_FpsHistory[500]{ 0 };
};
