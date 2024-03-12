#pragma once

#include "App.h"

namespace Engine 
{
	class AppManager
	{
	public:
		static void RunApplication(App* app);
		static App* GetRunningApplication();
		static void QuitApplication(int returnValue = 0);
		static int GetReturnValue();
	private:
		static App* s_RunningApplication;
		static bool s_IsRunning;
		static int s_ReturnValue;
	};
}
