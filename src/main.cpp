#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <Windows.h>

#include "App/Window.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Utils/Exception.h"

int main(int argc, char** argv) 
{
	try
	{
		Window window(1024, 680, "Flappy Bird Game", false);
		Shader shader("res/shaders/shader.vert", "res/shaders/shader.frag");
		Texture2D birdTexture("res/sprites/yellowbird-midflap.png");
		Texture2D birdUpflap("res/sprites/yellowbird-upflap.png");
		Texture2D birdDownflap("res/sprites/yellowbird-downflap.png");

		struct Vertex
		{
			glm::vec3	Position;
			glm::vec2	TexCoord;
			glm::vec4	Color;
		};

		const Vertex vertices[] = {
			{ { -0.1f, -0.1f, 0.0f }, { 0.0f, 0.0f }, glm::vec4(1.0f) },
			{ {  0.1f, -0.1f, 0.0f }, { 1.0f, 0.0f }, glm::vec4(1.0f) },
			{ {  0.1f,  0.1f, 0.0f }, { 1.0f, 1.0f }, glm::vec4(1.0f) },
			{ { -0.1f,  0.1f, 0.0f }, { 0.0f, 1.0f }, glm::vec4(1.0f) },
		};
		const unsigned short indices[] = { 0, 1, 2, 0, 2, 3 };

		unsigned int VBO, VAO, EBO;
		glCreateBuffers(1, &VBO);
		glCreateBuffers(1, &EBO);
		glCreateVertexArrays(1, &VAO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoord));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		unsigned int counter = 0;
		unsigned int subCounter = 0;

		while (window.IsOpen())
		{
			shader.Activate();

			glBindVertexArray(VAO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

			if (counter % 2 == 0) birdTexture.Activate();
			else if (counter % 4 == 1) birdUpflap.Activate();
			else if (counter % 4 == 3) birdDownflap.Activate();

			glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_SHORT, (const void*)0);

			window.SwapBuffers();
			window.PollEvents();

			if (++subCounter % 10 == 0) ++counter;
		}

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
