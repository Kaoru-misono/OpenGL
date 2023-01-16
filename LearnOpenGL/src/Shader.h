#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& VertexShaderPath, const std::string FragmentShaderPath);
	~Shader();

	//use Program
	void Bind();
	void UnBind();

	//Set Uniform
	void SetFloat(const std::string& name, float value);
	void SetFloat4(const std::string& name, const glm::vec4 values);

private:
	std::string ReadFile(const std::string& FilePath);
	void Compile(const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc);
private:
	GLint m_ShaderProgram;
};