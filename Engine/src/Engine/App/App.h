#pragma once

#include "Window.h"

namespace Engine 
{
	class App 
	{
	public:
		App();
		virtual ~App();
		App(const App&) = delete;
		App& operator=(const App&) = delete;

		virtual void Start();
		virtual void Update();

		Window* GetWindow() const noexcept;

	protected:
		Window* m_Window;
	};
}
