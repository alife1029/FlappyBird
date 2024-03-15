#include "engine_pch.h"

#include "AppManager.h"

namespace Engine
{
	App* AppManager::s_RunningApplication = nullptr;
	bool AppManager::s_IsRunning = false;
	int AppManager::s_ReturnValue = -1;

	void AppManager::RunApplication(App* app)
	{
		assert(!s_IsRunning);

		s_RunningApplication = app;
		s_IsRunning = true;

		app->Start();
		while (s_IsRunning)
		{
			app->Update();
		}

		QuitApplication();
	}
	
	void AppManager::QuitApplication(int returnValue)
	{
		s_RunningApplication = nullptr;
		s_IsRunning = false;
		s_ReturnValue = returnValue;
	}

	App* AppManager::GetRunningApplication()
	{
		return s_RunningApplication;
	}

	int AppManager::GetReturnValue()
	{
		assert(!s_IsRunning);
		return s_ReturnValue;
	}
}
