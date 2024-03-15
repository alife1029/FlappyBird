#include <Windows.h>
#include <Engine.h>

#include "FlappyBirdApp.h"

int APIENTRY WinMain(	_In_		HINSTANCE	hInstance,
						_In_opt_	HINSTANCE	hPrevInstance,
						_In_		LPSTR		lpCmdLine,
						_In_		int			nShowCmd)
{
	try
	{
		Engine::App* app = new FlappyBirdApp();
		Engine::AppManager::RunApplication(app);
		delete app;
	}
	catch (const Engine::EngineException& ex)
	{
		MessageBoxA(NULL, ex.what(), ex.GetType(), MB_ICONERROR);
	}
	catch (const Engine::EngineWException& ex)
	{
		MessageBoxW(NULL, ex.what_unicode(), ex.GetType(), MB_ICONERROR);
	}
	catch (const std::exception& ex)
	{
		MessageBoxA(NULL, ex.what(), "Standard Library Exception", MB_ICONERROR);
	}
	catch (...)
	{
		MessageBoxA(NULL, "No details", "Undefined Exception", MB_ICONERROR);
	}
	
	return Engine::AppManager::GetReturnValue();
}
