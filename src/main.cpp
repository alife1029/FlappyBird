#include <iostream>

#include <Windows.h>

#include "App/Window.h"
#include "Renderer/ShaderProgram.h"
#include "Utils/Exception.h"

int main(int argc, char** argv) 
{
	try
	{
		Window::Create(1024, 680, "Flappy Bird Game", false);

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
	catch (Exception& e)
	{
		MessageBox(NULL, e.what(), e.GetType(), MB_ICONERROR);
	}
	catch (std::exception& e)
	{
		MessageBox(NULL, e.what(), "Error", MB_ICONERROR);
	}
	catch (...)
	{
		MessageBox(NULL, "Something went wrong!", "Unknown Error", MB_ICONERROR);
	}
}
