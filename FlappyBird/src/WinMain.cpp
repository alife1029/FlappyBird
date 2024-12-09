#ifdef ENGINE_PLATFORM_WINDOWS
#include <Windows.h>
#elif defined(ENGINE_PLATFORM_UNIX)
#include <iostream>
#endif
#include <Engine.h>

#include "FlappyBirdApp.h"

#ifdef ENGINE_PLATFORM_WINDOWS
int APIENTRY WinMain(	_In_		HINSTANCE	hInstance,
						_In_opt_	HINSTANCE	hPrevInstance,
						_In_		LPSTR		lpCmdLine,
						_In_		int			nShowCmd)
#elif defined(ENGINE_PLATFORM_UNIX)
int main(int argc, const char** argv)
#endif
{
	try
	{
		Engine::App* app = new FlappyBirdApp();
		Engine::AppManager::RunApplication(app);
		delete app;
	}
	catch (const Engine::EngineException& ex)
	{
#ifdef ENGINE_PLATFORM_WINDOWS
		MessageBoxA(NULL, ex.what(), ex.GetType(), MB_ICONERROR);
#else
		std::cout << ex.GetType() << std::endl << ex.what() << std::endl;
#endif
	}
#ifdef ENGINE_PLATFORM_WINDOWS
	catch (const Engine::EngineWException& ex)
	{
		MessageBoxW(NULL, ex.what_unicode(), ex.GetType(), MB_ICONERROR);
	}
#endif
	catch (const std::exception& ex)
	{
#ifdef ENGINE_PLATFORM_WINDOWS
		MessageBoxA(NULL, ex.what(), "Standard Library Exception", MB_ICONERROR);
#else
		std::cout << "Standard Library Exception" << std::endl << ex.what() << std::endl;
#endif
		
	}
	catch (...)
	{
#ifdef ENGINE_PLATFORM_WINDOWS
		MessageBoxA(NULL, "No details", "Undefined Exception", MB_ICONERROR);
#else
		std::cout << "Undefined Exception" << std::endl << "No details" << std::endl;
#endif
	}
	
	return Engine::AppManager::GetReturnValue();
}
