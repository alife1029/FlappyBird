#include "FlappyBirdApp.h"

FlappyBirdApp::FlappyBirdApp()
{
	m_Window = new Engine::Window(1024, 720, "Flappy Bird", false);
}

FlappyBirdApp::~FlappyBirdApp()
{
	delete m_Window;
}

void FlappyBirdApp::Start()
{
	m_Window->Show();
}

void FlappyBirdApp::Update()
{
	m_Window->ProcessEvents();
}
