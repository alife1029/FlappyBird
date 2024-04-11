#include "FlappyBirdApp.h"
#include <ctime>
#include <cstdlib>

using namespace Engine;
using namespace glm;

FlappyBirdApp::FlappyBirdApp()
	:
	m_Shader(nullptr), m_Renderer(nullptr), m_Viewport(nullptr), m_Camera(nullptr), m_BackgroundTexture(nullptr), m_Bg(nullptr), 
	m_Bird(nullptr), m_PipeTexture(nullptr), m_PixelifySans(nullptr), m_Righteous(nullptr), m_TextRenderer(nullptr), m_TextShader(nullptr)
{
	m_Window = new Window(800, 600, "Flappy Bird", false);
}

FlappyBirdApp::~FlappyBirdApp()
{
	// Delete game objects
	delete m_Bg;
	delete m_Bird;
	for (auto p : m_Pipes) delete p;
	m_Pipes = {};

	// Delete textures
	for (auto charTex : m_CharacterTextures) delete charTex;
	m_CharacterTextures = {};
	delete m_BackgroundTexture;
	delete m_PipeTexture;

	// Delete fonts
	delete m_PixelifySans;

	// Delete rendering components
	delete m_Camera;
	delete m_Viewport;
	delete m_Shader;
	delete m_TextShader;
	delete m_TextRenderer;
	delete m_Renderer;
	delete m_Window;
}

void FlappyBirdApp::Start()
{
	App::Start();

	Input::SetupEventWindow(m_Window);

	m_Window->Show();
	m_Window->CreateGraphicsContext();

	// Create rendering components
	m_Shader = new Shader("res/shaders/sprite_vs.glsl", "res/shaders/sprite_fs.glsl");
	m_TextShader = new Shader("res/shaders/font_vs.glsl", "res/shaders/font_fs.glsl");
	m_Renderer = new BatchRenderer(m_Shader);
	m_TextRenderer = new TextRenderer(m_TextShader);
	m_Viewport = new Viewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Camera = new Camera(m_Viewport);
	m_Camera->zNear = 0.0f;

	// Loading textures
	m_BackgroundTexture = new Texture2D("res/sprites/background-day.png");
	m_PipeTexture = new Texture2D("res/sprites/pipe-green.png");
	m_CharacterTextures = {
		new Texture2D("res/sprites/yellowbird-downflap.png"),
		new Texture2D("res/sprites/yellowbird-midflap.png"),
		new Texture2D("res/sprites/yellowbird-upflap.png"),
	};

	// Loading fonts
	m_PixelifySans = new Font("res/fonts/PixelifySans.ttf", 128u, Texture2D::Filter::Point);
	m_Righteous = new Font("res/fonts/Righteous.ttf");

	// Create game objects
	m_Bg = new Background(m_BackgroundTexture, { -1.0f, 0.0f, 0.0f });
	m_Bird = new Bird(&m_CharacterTextures[0]);
	m_Pipes = { 
		new Pipe(m_PipeTexture, 5.0f,  { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 7.0f,  { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 9.0f,  { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 11.0f, { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 13.0f, { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 15.0f, { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 17.0f, { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 19.0f, { -1.0f, 0.0f, 0.0f }, m_Bird),
		new Pipe(m_PipeTexture, 21.0f, { -1.0f, 0.0f, 0.0f }, m_Bird)
	};

	// Random seed
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
}
  
void FlappyBirdApp::Update()
{
	App::Update();

	// Game logic update
	m_Bg->Update();
	m_Bird->Update(m_ElapsedTime);
	for (Pipe* pipe : m_Pipes)
		pipe->Update();

	m_Camera->Update();

	// Render game
	m_Window->GetGfx()->ClearBuffer(0.0f, 0.0f, 0.0f, 1.0f);

	m_Renderer->Begin(m_Camera->GetCombinedMatrix());
	m_Bg->Render(m_Renderer);
	m_Bird->Render(m_Renderer);
	for (Pipe* pipe : m_Pipes)
		pipe->Render(m_Renderer);
	m_Renderer->End();

	m_TextRenderer->Begin({ m_Window->GetWidth(), m_Window->GetHeight() });
	//m_TextRenderer->Draw(m_Righteous, "ABCDEFGHIJKLMN ", { 150, 150 }, 72.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	m_TextRenderer->DrawChar(m_Righteous, 'A', { 0, 0 }, 75.0f);
	m_TextRenderer->DrawChar(m_PixelifySans, 'D', { 100, 100 }, 75.0f);
	m_TextRenderer->Draw(m_Righteous, "Hello World", { 300, 300 }, 36.0f);
	m_TextRenderer->Draw(m_Righteous, "A B", { 25, 500 }, 36.0f);
	m_TextRenderer->Draw(m_Righteous, "HELLO FLAPPY BIRD C++", { 25, 400 }, 36.0f);
 	m_TextRenderer->End();

	m_Window->GetGfx()->EndFrame();
	m_Window->ProcessEvents();

	m_Viewport->SetDimensions(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Viewport->Apply();

	m_ElapsedTime += Time::Delta();
}
