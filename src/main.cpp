#include <iostream>

#include "App/Window.h"
#include "Renderer/ShaderProgram.h"

int main(int argc, char** argv) 
{
	Window::CreateWindow(1024, 680, "Flappy Bird Game", false);

	Shader* shader = new Shader("res/shaders/shader.vert", "res/shaders/shader.frag");
	
	while (Window::IsOpen())
	{
		shader->Activate();

		Window::SwapBuffers();
		Window::PollEvents();
	}

	delete shader;

	return 0;
}
