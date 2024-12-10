#include "engine_pch.h"
#include "ImGuiManager.h"

#include <imgui.h>
#ifdef ENGINE_PLATFORM_WINDOWS
#include <imgui_impl_win32.h>
#elif defined(ENGINE_PLATFORM_UNIX)
#include "Engine/Platform/Linux/GLFWWindow.h"
#include <imgui_impl_glfw.h>
#endif
#include <imgui_impl_opengl3.h>

namespace Engine
{
	void ImGuiManager::Initialize(Window* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui::StyleColorsDark();
#ifdef ENGINE_PLATFORM_WINDOWS
		ImGui_ImplWin32_InitForOpenGL((void*)window->GetHWND());
#elif defined(ENGINE_PLATFORM_UNIX)
		ImGui_ImplGlfw_InitForOpenGL(((GLFWWindow*)(window))->GetGLFWwindow(), false);
#endif
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void ImGuiManager::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
#ifdef ENGINE_PLATFORM_WINDOWS
		ImGui_ImplWin32_Shutdown();
#elif defined(ENGINE_PLATFORM_UNIX)
		ImGui_ImplGlfw_Shutdown();
#endif
		ImGui::DestroyContext();
	}

	void ImGuiManager::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
#ifdef ENGINE_PLATFORM_WINDOWS
		ImGui_ImplWin32_NewFrame();
#elif defined(ENGINE_PLATFORM_UNIX)
		ImGui_ImplGlfw_NewFrame();
#endif
		ImGui::NewFrame();
	}
	
	void ImGuiManager::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
