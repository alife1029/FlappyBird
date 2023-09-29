#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int main(int argc, char** argv) 
{
	if (!glfwInit()) 
	{
		std::cout << "Failed to initialize GLFW!" << std::endl;
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "Flappy Bird", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Load Modern OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load Modern OpenGL!" << std::endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	// V-Sync
	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}
