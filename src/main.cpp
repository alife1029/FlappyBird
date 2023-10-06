#include <iostream>

#include "App/Window.h"

int main(int argc, char** argv) 
{
	Window::CreateWindow(1024, 680, "Flappy Bird Game", false);

	while (Window::IsOpen())
	{
		Window::SwapBuffers();
		Window::PollEvents();
	}

	return 0;
}
