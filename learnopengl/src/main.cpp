#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, true);
}

void OnResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

struct ShaderSourceCode
{
	std::string VertexShaderSourceCode;
	std::string PixelShaderSourceCode;
};

ShaderSourceCode ParseShaderFile(const std::string& filePath)
{
	std::ifstream ifs(filePath);
	
	enum  class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		PIXEL = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType shaderType = ShaderType::NONE;
	while (getline(ifs, line))
	{
		if (line.find("shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				shaderType = ShaderType::VERTEX;
			}
			else if (line.find("pixel") != std::string::npos)
			{
				shaderType = ShaderType::PIXEL;
			}
		}
		else
		{
			ss[(int)shaderType] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };
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
	ShaderSourceCode shaderSourceCode;
	shaderSourceCode = ParseShaderFile("D:/C++Project/learnopengl/learnopengl/shader/Basic.shader");
	unsigned int program = CreateProgram(shaderSourceCode.VertexShaderSourceCode, shaderSourceCode.PixelShaderSourceCode);

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

	glDeleteProgram(program);

	glfwTerminate();
	return 0;
}