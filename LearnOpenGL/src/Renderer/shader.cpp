#include "shader.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertex_shader_path, const std::string fragment_shader_path)
{
	unsigned int shader_program = glCreateProgram();
	m_render_ID = shader_program;
	std::string vertex_shader_src = read_file(vertex_shader_path);
	std::string fragment_shader_src = read_file(fragment_shader_path);
	compile(vertex_shader_src, fragment_shader_src);
}

Shader::~Shader()
{
	glDeleteProgram(m_render_ID);
}

void Shader::bind() const
{
	glUseProgram(m_render_ID);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::set_float(const std::string& name, float value)
{
	GLint location = glGetUniformLocation(m_render_ID, name.c_str());
	glUniform1f(location, value);
}

void Shader::set_vec3(const std::string& name, const glm::vec3 values)
{
	GLint location = glGetUniformLocation(m_render_ID, name.c_str());
	glUniform3f(location, values.x, values.y, values.z);
}

void Shader::set_vec4(const std::string& name, const glm::vec4 values)
{
	GLint location = glGetUniformLocation(m_render_ID, name.c_str());
	glUniform4f(location, values.x, values.y, values.z, values.w);
}

void Shader::set_int(const std::string& name, int value)
{
	GLint location = glGetUniformLocation(m_render_ID, name.c_str());
	glUniform1i(location, value);
}

void Shader::set_mat4(const std::string& name, const glm::mat4 values)
{
	GLint location = glGetUniformLocation(m_render_ID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(values));
}

std::string Shader::read_file(const std::string& FilePath)
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

void Shader::compile(const std::string& vertex_shader_src, const std::string& fragment_shader_src)
{
	//--------------Create and Compile Shader-----------------------
	unsigned int vertex_shader, fragment_shader;

	// Create an empty vertex shader handle
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	// Send the vertex shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	const GLchar* source = vertex_shader_src.c_str();
	glShaderSource(vertex_shader, 1, &source, 0);
	// Compile the vertex shader
	glCompileShader(vertex_shader);

	GLint is_compiled = 0;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
	if (is_compiled == GL_FALSE)
	{
		GLint max_length = 0;
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);

		// The max_length includes the NULL character
		std::vector<GLchar> infoLog(max_length);
		glGetShaderInfoLog(vertex_shader, max_length, &max_length, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(vertex_shader);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		std::cout << infoLog.data() << std::endl;
		std::cout << "VertexShader Compilation failed!" << std::endl;
		return;
	}

	// Create an empty Fragment shader handle
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	// Send the Fragment shader source code to GL
	// Note that std::string's .c_str is NULL character terminated.
	source = fragment_shader_src.c_str();
	glShaderSource(fragment_shader, 1, &source, 0);
	// Compile the Fragment shader
	glCompileShader(fragment_shader);

	is_compiled = 0;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
	if (is_compiled == GL_FALSE)
	{
		GLint max_length = 0;
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);

		// The max_length includes the NULL character
		std::vector<GLchar> infoLog(max_length);
		glGetShaderInfoLog(fragment_shader, max_length, &max_length, &infoLog[0]);

		// We don't need the shader anymore.
		glDeleteShader(fragment_shader);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		std::cout << infoLog.data() << std::endl;
		std::cout << "FragmentShader Compilation failed!" << std::endl;
		return;
	}

	glAttachShader(m_render_ID, vertex_shader);
	glAttachShader(m_render_ID, fragment_shader);
	glLinkProgram(m_render_ID);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(m_render_ID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(m_render_ID, GL_INFO_LOG_LENGTH, &maxLength);

		// The max_length includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_render_ID, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(m_render_ID);
		// Don't leak shaders either.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		std::cout << infoLog.data() << std::endl;
		std::cout << "Shader link failed!" << std::endl;

		return;
	}

	// Always detach shaders after a successful link.
	glDetachShader(m_render_ID, vertex_shader);
	glDetachShader(m_render_ID, fragment_shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}
