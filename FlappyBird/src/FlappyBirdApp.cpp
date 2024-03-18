#include "FlappyBirdApp.h"

FlappyBirdApp::FlappyBirdApp()
{
	m_Window = new Engine::Window(640, 480, "Flappy Bird", false);
}

FlappyBirdApp::~FlappyBirdApp()
{
	delete m_Window;
}

void FlappyBirdApp::Start()
{
	m_Window->Show();
	m_Window->CreateGraphicsContext();
}

void FlappyBirdApp::Update()
{
	m_Window->GetGfx()->ClearBuffer(1.0f, 0.2f, 0.3f, 1.0f);
	m_Window->GetGfx()->EndFrame();
	m_Window->ProcessEvents();
}
