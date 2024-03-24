#include "FlappyBirdApp.h"

using namespace Engine;
using namespace glm;

FlappyBirdApp::FlappyBirdApp()
	:
	m_Shader(nullptr), m_Renderer(nullptr), m_Viewport(nullptr), m_Camera(nullptr), m_BackgroundTexture(nullptr), m_Bg(nullptr)
{
	m_Window = new Window(800, 600, "Flappy Bird", false);
}

FlappyBirdApp::~FlappyBirdApp()
{
	for (Texture2D* charTex : m_CharacterTextures)
	{
		delete charTex;
	}
	m_CharacterTextures = {};

	delete m_BackgroundTexture;
	delete m_Camera;
	delete m_Viewport;
	delete m_Shader;
	delete m_Renderer;
	delete m_Window;
}

void FlappyBirdApp::Start()
{
	App::Start();

	Input::SetupEventWindow(m_Window);

	m_Window->Show();
	m_Window->CreateGraphicsContext();

	m_Shader = new Shader("res/shaders/sprite_vs.glsl", "res/shaders/sprite_fs.glsl");
	m_Renderer = new BatchRenderer(m_Shader);
	m_Viewport = new Viewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Camera = new Camera(m_Viewport);
	m_Camera->zNear = 0.0f;
	m_BackgroundTexture = new Texture2D("res/sprites/background-day.png");
	m_CharacterTextures = {
		new Texture2D("res/sprites/yellowbird-downflap.png"),
		new Texture2D("res/sprites/yellowbird-midflap.png"),
		new Texture2D("res/sprites/yellowbird-upflap.png"),
	};

	m_Bg = new Background(m_BackgroundTexture, { -1.0f, 0.0f, 0.0f });
}

void FlappyBirdApp::Update()
{
	App::Update();

	// Game logic update
	m_BirdPosition += m_BirdVelocity * Time::Delta();
	m_BirdVelocity.y -= 9.807f * Time::Delta();
	if (Input::IsKeyPressed(Key::Space))
	{
		m_BirdVelocity.y = 4.0f;
	}
	m_BirdRotationZ = 45.0f * m_BirdVelocity.y / 10.0f;
	m_Bg->Update();

	m_Camera->Update();

	m_Window->GetGfx()->ClearBuffer(0.0f, 0.0f, 0.0f, 1.0f);

	m_Renderer->Begin(m_Camera->GetCombinedMatrix());
	m_Bg->Render(m_Renderer);
	m_Renderer->DrawTexture(m_CharacterTextures[static_cast<size_t>(m_ElapsedTime * 8.0f) % 3], m_BirdPosition, m_BirdRotationZ);
	m_Renderer->End();

	m_Window->GetGfx()->EndFrame();
	m_Window->ProcessEvents();

	m_Viewport->SetDimensions(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Viewport->Apply();

	m_ElapsedTime += Time::Delta();
}
