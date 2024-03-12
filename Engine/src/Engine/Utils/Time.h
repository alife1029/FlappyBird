#pragma once

namespace Engine
{
	class Time
	{
		friend class App;
	public:
		static float Delta();
		static float Elapsed();
	private:
		static void Update();
	};
}
