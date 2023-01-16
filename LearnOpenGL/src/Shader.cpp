#include "Shader.h"
#include <fstream>
#include <iostream>
#include <vector>

Shader::Shader(const std::string& VertexShaderPath, const std::string FragmentShaderPath)
{
	m_ShaderProgram = glCreateProgram();
	std::string VertexShaderSrc = ReadFile(VertexShaderPath);
	std::string FragmentShaderSrc = ReadFile(FragmentShaderPath);
	Compile(VertexShaderSrc, FragmentShaderSrc);
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderProgram);
}

void Shader::Bind()
{
	glUseProgram(m_ShaderProgram);
}

void Shader::UnBind()
{
	glUseProgram(0);
}

void Shader::SetFloat(const std::string& name, float value)
{
	GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
	glUniform1f(location, value);
}

void Shader::SetFloat4(const std::string& name, const glm::vec4 values)
{
	GLint location = glGetUniformLocation(m_ShaderProgram, name.c_str());
	glUniform4f(location, values.x, values.y, values.z, values.w);
}

std::string Shader::ReadFile(const std::string& FilePath)
{
	std::string ShaderSrc;
	std::ifstream in(FilePath, std::ios::in, std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		if (size != -1)
		{
			ShaderSrc.resize(size);
			in.seekg(0, std::ios::beg);
			in.read(&ShaderSrc[0], size);
			in.close();
		}
		else
			std::cout << "Could not read from file " << FilePath << std::endl;
	}
	else
		std::cout << "Could not open the file! " << std::endl;
	return ShaderSrc;
}

void Shader::Compile(const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc)
{
	//--------------Create and Compile Shader-----------------------
	unsigned int VertexShader, FragmentShader;

	// Create an empty vertex shader handle
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = VertexShaderSrc.c_str();
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
		return;
	}

	// Create an empty Fragment shader handle
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Send the Fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = FragmentShaderSrc.c_str();
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
		return;
	}

	glAttachShader(m_ShaderProgram, VertexShader);
	glAttachShader(m_ShaderProgram, FragmentShader);
	glLinkProgram(m_ShaderProgram);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_ShaderProgram, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(m_ShaderProgram);
		// Don't leak shaders either.
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		std::cout << infoLog.data() << std::endl;
		std::cout << "Shader link failed!" << std::endl;

		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(m_ShaderProgram, VertexShader);
	glDetachShader(m_ShaderProgram, FragmentShader);

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}
