#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}

void OnResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	//gladLoadGL();
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, OnResize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		//std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glfwSetKeyCallback(window, KeyCallBack);

		/* Render here */
		glClearColor(0.5f, 0.5f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		
		//glGenBuffers()

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	} 

	glfwTerminate();
	return 0;
}