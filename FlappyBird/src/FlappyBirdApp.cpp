#include "FlappyBirdApp.h"

FlappyBirdApp::FlappyBirdApp()
	:
	m_Shader(nullptr), m_Renderer(nullptr), m_Viewport(nullptr), m_Camera(nullptr), m_BackgroundTexture(nullptr)
{
	m_Window = new Engine::Window(800, 600, "Flappy Bird", false);
}

FlappyBirdApp::~FlappyBirdApp()
{
	for (Engine::Texture2D* charTex : m_CharacterTextures)
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

	Engine::Input::SetupEventWindow(m_Window);

	m_Window->Show();
	m_Window->CreateGraphicsContext();

	m_Shader = new Engine::Shader("res/shaders/sprite_vs.glsl", "res/shaders/sprite_fs.glsl");
	m_Renderer = new Engine::BatchRenderer(m_Shader);
	m_Viewport = new Engine::Viewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Camera = new Engine::Camera(m_Viewport);
	m_Camera->zNear = 0.0f;
	m_BackgroundTexture = new Engine::Texture2D("res/sprites/background-day.png");
	m_CharacterTextures = {
		new Engine::Texture2D("res/sprites/yellowbird-downflap.png"),
		new Engine::Texture2D("res/sprites/yellowbird-midflap.png"),
		new Engine::Texture2D("res/sprites/yellowbird-upflap.png"),
	};
}

void FlappyBirdApp::Update()
{
	App::Update();

	// Game logic update
	m_BirdPosition += m_BirdVelocity * Engine::Time::Delta();
	m_BirdVelocity.y -= 9.807f * Engine::Time::Delta();
	if (Engine::Input::IsKeyPressed(Engine::Key::Space))
	{
		m_BirdVelocity.y = 4.0f;
	}
	m_BirdRotationZ = 45.0f * m_BirdVelocity.y / 10.0f;

	m_Camera->Update();

	m_Window->GetGfx()->ClearBuffer(0.0f, 0.0f, 0.0f, 1.0f);

	m_Renderer->Begin(m_Camera->GetCombinedMatrix());
	m_Renderer->DrawTexture(m_BackgroundTexture, { 0.0f, 0.0f, 0.0f });
	m_Renderer->DrawTexture(m_CharacterTextures[static_cast<size_t>(m_ElapsedTime * 8.0f) % 3], m_BirdPosition, m_BirdRotationZ);
	m_Renderer->End();

	m_Window->GetGfx()->EndFrame();
	m_Window->ProcessEvents();

	m_Viewport->SetDimensions(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Viewport->Apply();

	static int x;
	if (++x % 30 == 0) m_Window->SetTitle(std::to_string((int)(1 / Engine::Time::Delta())) + " FPS");

	m_ElapsedTime += Engine::Time::Delta();
}
