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
	void bind() const;
	void unbind() const;

	//Set Uniform
	void set_float(const std::string& name, float value);
	void set_float4(const std::string& name, const glm::vec4 values);
	void set_int(const std::string& name, int value);


private:
	std::string read_file(const std::string& FilePath);
	void compile(const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc);
private:
	GLint m_render_ID;
};