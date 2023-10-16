#include <iostream>

#include <Windows.h>

#include "App/Window.h"
#include "Renderer/ShaderProgram.h"
#include "Utils/Exception.h"

int main(int argc, char** argv) 
{
	try
	{
		Window window(1024, 680, "Flappy Bird Game", false);

		Shader* shader = new Shader("res/shaders/shader.vert", "res/shaders/shader.frag");

		while (window.IsOpen())
		{
			shader->Activate();

			window.SwapBuffers();
			window.PollEvents();
		}

		delete shader;

		return 0;
	}
	catch (const Exception& e)
	{
		MessageBoxA(NULL, e.what(), e.GetType(), MB_ICONERROR);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Error", MB_OK | MB_ICONERROR);
	}
	catch (...)
	{
		MessageBoxA(NULL, "Something went wrong!", "Unknown Error", MB_OK | MB_ICONERROR);
	}

	return -1;
}
