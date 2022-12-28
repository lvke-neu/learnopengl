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

unsigned int CompileShader(unsigned int shaderType, const char* shaderSourceCode)
{
	unsigned int shader;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSourceCode, nullptr);
	glCompileShader(shader);

	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int infoLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

		char* errorInfo = new char[infoLength];
		glGetShaderInfoLog(shader, infoLength, &infoLength, errorInfo);
		std::cout << errorInfo << std::endl;
		delete[] errorInfo;

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

unsigned int CreateProgram(const std::string& vsShaderCode, const std::string& psShaderCode)
{
	unsigned int program = glCreateProgram();
	unsigned int vsShader = CompileShader(GL_VERTEX_SHADER, vsShaderCode.c_str());
	unsigned int psShader = CompileShader(GL_FRAGMENT_SHADER, psShaderCode.c_str());

	glAttachShader(program, vsShader);
	glAttachShader(program, psShader);
	glLinkProgram(program);
	glValidateProgram(program);

	int result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		int infoLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);

		char* errorInfo = new char[infoLength];
		glGetProgramInfoLog(program, infoLength, &infoLength, errorInfo);
		std::cout << errorInfo << std::endl;
		delete[] errorInfo;

		glDeleteProgram(program);
		return 0;
	}

	glDeleteShader(vsShader);
	glDeleteShader(psShader);

	return program;
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

	std::cout << "GL Version" << glGetString(GL_VERSION) << std::endl;
	std::cout << "GL Version" << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GL Version" << glGetString(GL_RENDERER) << std::endl;

	//vertexbuff
	float triPosition[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5,  0.0f,
		 0.0f,  0.5f, 0.0f
	};
	unsigned int vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triPosition), triPosition, GL_STATIC_DRAW);

	//layout
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false ,3 * sizeof(float), (void*)0);
	

 
	//shader
	std::string vsShaderCode =
		"#version 330 core \n"
		"layout(location = 0) in vec3 position;\n"
		"void main() \n"
		"{\n"
		"gl_Position = vec4(position.xyz, 1.0);"
		"}\n";
	std::string psShaderCode =
		"#version 330 core \n"
		"out vec4 pixelColor;\n"
		"void main() \n"
		"{\n"
		"pixelColor = vec4(1.0,0.0,0.0, 1.0);"
		"}\n";
	unsigned int program = CreateProgram(vsShaderCode, psShaderCode);

	glUseProgram(program);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glfwSetKeyCallback(window, KeyCallBack);

		/* Render here */
		glClearColor(0.5f, 0.5f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		//draw
		glDrawArrays(GL_TRIANGLES, 0, 3);
		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	} 

	glfwTerminate();
	return 0;
}