#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& VertexShaderPath, const std::string& FragmentShaderPath);
	~Shader();

	//use Program
	void bind() const;
	void unbind() const;

	//Set Uniform
	void set_float(const std::string& name, float value);
	void set_vec3(const std::string& name, const glm::vec3 values);
	void set_vec4(const std::string& name, const glm::vec4 values);
	void set_int(const std::string& name, int value);
	void set_mat4(const std::string& name, const glm::mat4 values);



private:
	std::string read_file(const std::string& FilePath);
	void compile(const std::string& VertexShaderSrc, const std::string& FragmentShaderSrc);
private:
	GLint m_render_ID;
};