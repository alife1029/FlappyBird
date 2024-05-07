#include "FlappyBirdApp.h"
#include <ctime>
#include <cstdlib>

using namespace Engine;
using namespace glm;

FlappyBirdApp::FlappyBirdApp()
	:
	m_Shader(nullptr), m_Renderer(nullptr), m_Viewport(nullptr), m_Camera(nullptr), m_BackgroundTexture(nullptr), m_Bg(nullptr), 
	m_Bird(nullptr), m_PipeTexture(nullptr), m_PixelifySans(nullptr), m_Righteous(nullptr), m_UiRenderer(nullptr), m_TextShader(nullptr),
	m_UiImageShader(nullptr)
{
	m_Window = new Window(800, 600, "Flappy Bird", false);
}

FlappyBirdApp::~FlappyBirdApp()
{
	// Shutdown ImGui
	ImGuiManager::Shutdown();

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
	delete m_WhiteTexture;

	// Delete fonts
	delete m_PixelifySans;

	// Delete rendering components
	delete m_Camera;
	delete m_Viewport;
	delete m_Shader;
	delete m_TextShader;
	delete m_UiImageShader;
	delete m_UiRenderer;
	delete m_Renderer;
	delete m_Window;
}

void FlappyBirdApp::Start()
{
	App::Start();

	Input::SetupEventWindow(m_Window);

	m_Window->Show();
	m_Window->CreateGraphicsContext();

	// Initialize ImGui
	ImGuiManager::Initialize(m_Window);

	// Create rendering components
	m_Shader = new Shader("res/shaders/sprite_vs.glsl", "res/shaders/sprite_fs.glsl");
	m_TextShader = new Shader("res/shaders/ui_vs.glsl", "res/shaders/text_fs.glsl");
	m_UiImageShader = new Shader("res/shaders/ui_vs.glsl", "res/shaders/sprite_fs.glsl");
	m_Renderer = new BatchRenderer(m_Shader);
	m_UiRenderer = new UIRenderer(m_TextShader, m_UiImageShader);
	m_Viewport = new Viewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Camera = new Camera(m_Viewport);
	m_Camera->zNear = 0.0f;

	// Loading textures
	m_BackgroundTexture = new Texture2D("res/sprites/background-day.png");
	m_PipeTexture = new Texture2D("res/sprites/pipe-green.png");
	m_CharacterTextures = {
		new Texture2D("res/sprites/yellowbird-downflap.png", 100u, false, Texture2D::Filter::Point),
		new Texture2D("res/sprites/yellowbird-midflap.png", 100u, false, Texture2D::Filter::Point),
		new Texture2D("res/sprites/yellowbird-upflap.png", 100u, false, Texture2D::Filter::Point),
	};
	m_WhiteTexture = Texture2D::LoadWhiteTexture();

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

	if (m_GameRunning)
	{
		m_Bird->Update(m_ElapsedTime);
		for (Pipe* pipe : m_Pipes)
			pipe->Update();
	}
	else
	{
		if (Input::IsKeyJustPressed(Key::Enter))
		{
			StartGame();
		}
	}

	m_Camera->Update();
	
	// Render game
	m_Window->GetGfx()->ClearBuffer(0.0f, 0.0f, 0.0f, 1.0f);

	RenderObjects();
	RenderUI();

#ifdef _DEBUG
	RenderImGui();
#endif

	m_Window->GetGfx()->EndFrame();
	m_Window->ProcessEvents();

	m_Viewport->SetDimensions(0, 0, m_Window->GetWidth(), m_Window->GetHeight());
	m_Viewport->Apply();

	m_ElapsedTime += Time::Delta();
}

void FlappyBirdApp::StartGame()
{
	m_FirstLaunched = false;
	m_GameRunning = true;
}

void FlappyBirdApp::ResetGame()
{
	m_GameRunning = false;
}

void FlappyBirdApp::RenderObjects()
{
	m_Renderer->Begin(m_Camera->GetCombinedMatrix());
	m_Bg->Render(m_Renderer);
	if (m_GameRunning)
	{
		m_Bird->Render(m_Renderer);
		for (Pipe* pipe : m_Pipes)
			pipe->Render(m_Renderer);
	}
	m_Renderer->End();
}

void FlappyBirdApp::RenderUI()
{
	m_UiRenderer->Begin({ m_Window->GetWidth(), m_Window->GetHeight() });

	// Render menu UI when the game is not running
	if (!m_GameRunning)
	{
		// Draw background panel
		m_UiRenderer->DrawImage(
			m_WhiteTexture,
			{ 0.0f, 0.0f },
			{ m_Window->GetWidth() < 800 ? 480 : m_Window->GetWidth() * 0.6, m_Window->GetHeight() },
			0.0f,
			Anchor::MiddleCenter, Anchor::MiddleCenter,
			{ 0.1, 0.1, 0.1, 0.3 }
		);

		m_UiRenderer->DrawTxt(m_Righteous, "Flappy Bird", { 0.0f, 64.0f }, Anchor::TopCenter, 64.0f);
		m_UiRenderer->DrawTxt(m_PixelifySans, "Hi. Score: " + std::to_string(m_HiScore), { 0.0f, 64.0f }, Anchor::BottomCenter, 20.0f);

		if (m_FirstLaunched)
		{
			m_UiRenderer->DrawImage(m_CharacterTextures[static_cast<int>(m_ElapsedTime * 5) % 3], { 0.0f, 48.0f }, { 160.0f, 128.0f });
			m_UiRenderer->DrawTxt(m_PixelifySans, "v1.0.0", { 4.0, 4.0 }, Anchor::BottomRight);
			m_UiRenderer->DrawTxt(m_PixelifySans, "Press ENTER to Start Game", { 0.0f, -96.0f }, Anchor::MiddleCenter, 20.0f, { 0.05f, 0.05f, 0.05f, abs(sinf(m_ElapsedTime * 1.75f)) * 0.8f + 0.2f });
		}
		else
		{
			m_UiRenderer->DrawTxt(m_PixelifySans, "Press ENTER to Restart Game", { 0.0f, -96.0f }, Anchor::MiddleCenter, 20.0f, { 0.05f, 0.05f, 0.05f, abs(sinf(m_ElapsedTime * 1.75f)) * 0.8f + 0.2f });
		}
	}

	// Render in-game UI when the game is running
	else
	{
		m_UiRenderer->DrawTxt(m_PixelifySans, std::to_string(m_Score), { 0.0f, 16.0f }, Anchor::TopCenter, 96.0f);
	}
	
	m_UiRenderer->End();
}

void FlappyBirdApp::RenderImGui()
{
	ImGuiManager::NewFrame();
	ImGui::Begin("Rendering Stats");

	ImGui::Text("FPS: %g (%g ms)", 1.0f / Time::Delta(), 1000.0f * Time::Delta());

	// Update FPS plot
	for (int i = 1; i < std::size(m_FpsHistory); i++)
	{
		m_FpsHistory[i - 1] = m_FpsHistory[i];
	}
	m_FpsHistory[std::size(m_FpsHistory) - 1] = 1.0f / Time::Delta();
	ImGui::PlotLines("FPS", m_FpsHistory, (int)std::size(m_FpsHistory));

	ImGui::Text("Batch Count: %llu", RendererStats::GetBatchCount());
	ImGui::Text("Vertex Count: %llu", RendererStats::GetVertexCount());
	ImGui::Text("Traingle Count: %llu", RendererStats::GetTriangleCount());

	ImGui::End();
	ImGuiManager::EndFrame();
}
