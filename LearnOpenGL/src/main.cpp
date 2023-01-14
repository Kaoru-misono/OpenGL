#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	//Init
	//-----------------------------------------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	//------------------------------------------------------------------------

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(800, 600, "OPenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create window!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Glad Initialized failed!" << std::endl;
		return -1;
	}
	//------------------------------------------------------------------------------

	//What do we need to draw an object on screen?
	//Vertex Array, Vertex Buffer, Index Buffer and shader
	unsigned int VertexArray, VertexBuffer, IndexBuffer;

	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	//Create an array of vertices
	float vertices[4 * 3] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f
	};
	//array of vertices has Two Triangles 
	float verticesOfTwoTriangle[6 * 3] =
	{
		-0.9f, -0.5f, 0.0f,
		 0.0f, -0.5f, 0.0f,
		-0.45f, 0.5f, 0.0f,
		-0.0f, -0.5f, 0.0f,
		 0.9f, -0.5f, 0.0f,
		 0.45f, 0.5f, 0.0f
	};

	//Array of indices
	unsigned int indices[6] = 
	{ 
		0, 1, 2, 
		2, 3, 1
	};

	//Specify the contents of the Vertex buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesOfTwoTriangle), verticesOfTwoTriangle, GL_STATIC_DRAW);
	//Allows vertex shaders to read GPU (server-side) data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);



	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Shader
	//---------------Vertex Shader----------------------
	std::string VertexSrc = R"(
			#version 330 core		
			layout(location = 0) in vec3 a_Position;
			
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
			)";
	//--------------------------------------------------
	//--------------Fragment Shader---------------------
	std::string FragmentSrc = R"(
			#version 330 core
			layout(location = 0) out vec4 Fragcolor;
			
			in vec3 v_Position;
			void main()
			{
				Fragcolor = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
			)";
	//---------------------------------------------------

	//--------------Create and Compile Shader-----------------------
	unsigned int VertexShader, FragmentShader;

	// Create an empty vertex shader handle
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = VertexSrc.c_str();
	glShaderSource(VertexShader, 1, &source, 0);
	// Compile the vertex shader
	glCompileShader(VertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(VertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(VertexShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(VertexShader);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		std::cout << infoLog.data() << std::endl;
		std::cout << "VertexShader Compilation failed!" << std::endl;
		return -1;
	}

	// Create an empty Fragment shader handle
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Send the Fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = FragmentSrc.c_str();
	glShaderSource(FragmentShader, 1, &source, 0);
	// Compile the Fragment shader
	glCompileShader(FragmentShader);

	isCompiled = 0;
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(FragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(FragmentShader, maxLength, &maxLength, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(FragmentShader);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		std::cout << infoLog.data() << std::endl;
		std::cout << "FragmentShader Compilation failed!" << std::endl;
		return -1;
	}

	unsigned int ShaderProgram;
	ShaderProgram = glCreateProgram();

	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glLinkProgram(ShaderProgram);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(ShaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(ShaderProgram, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(ShaderProgram);
		// Don't leak shaders either.
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		std::cout << infoLog.data() << std::endl;
		std::cout << "Shader link failed!" << std::endl;

		return -1;
	}

	// Always detach shaders after a successful link.
	glDetachShader(ShaderProgram, VertexShader);
	glDetachShader(ShaderProgram, FragmentShader);

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);


	// uncomment this call to draw in wireframe polygons.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	//render loop
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.8f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(ShaderProgram);
		glBindVertexArray(VertexArray);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glfw: swap buffersand poll IO events(keys pressed / released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VertexArray);
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteBuffers(1, &IndexBuffer);
	glDeleteProgram(ShaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
   // ------------------------------------------------------------------
	glfwTerminate();
	return 0;

}