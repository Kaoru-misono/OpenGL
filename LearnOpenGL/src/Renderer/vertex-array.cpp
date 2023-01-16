#include "vertex-array.h"

#include <glad/glad.h>

static GLenum shader_data_type_to_OpenGL_type(Shader_Data_Type type)
{
	switch (type)
	{
	case Shader_Data_Type::Float:		return GL_FLOAT;
	case Shader_Data_Type::Float2:		return GL_FLOAT;
	case Shader_Data_Type::Float3:		return GL_FLOAT;
	case Shader_Data_Type::Float4:		return GL_FLOAT;
	case Shader_Data_Type::Mat3:		return GL_FLOAT;
	case Shader_Data_Type::Mat4:		return GL_FLOAT;
	case Shader_Data_Type::Int:			return GL_INT;
	case Shader_Data_Type::Int2:		return GL_INT;
	case Shader_Data_Type::Int3:		return GL_INT;
	case Shader_Data_Type::Int4:		return GL_INT;
	case Shader_Data_Type::Bool:		return GL_BOOL;
	}

	std::cout << "Unknown ShaderDataType!" <<std::endl;
	return 0;
}

Vertex_Array::Vertex_Array()
{
	glGenVertexArrays(1, &m_render_ID);
}

Vertex_Array::~Vertex_Array()
{
	glDeleteVertexArrays(1, &m_render_ID);
}

void Vertex_Array::bind() const
{
	glBindVertexArray(m_render_ID);
}

void Vertex_Array::unbind() const
{
	glBindVertexArray(0);
}

void Vertex_Array::add_vertex_buffer(const std::shared_ptr<Vertex_Buffer>& vertex_buffer)
{
	glBindVertexArray(m_render_ID);
	vertex_buffer->bind();

	//set vertex attribute
	uint32_t index = 0;
	const auto& layout = vertex_buffer->get_layout();
	for (const auto& element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index,
			element.get_value_count(),
			shader_data_type_to_OpenGL_type(element.type),
			element.normalized ? GL_TRUE : GL_FALSE,
			layout.get_stride(),
			(const void*)element.offset);
		index++;
	}
	m_vertex_buffers.push_back(vertex_buffer);
}

void Vertex_Array::set_index_buffer(const std::shared_ptr<Index_Buffer>& index_buffer)
{
	glBindVertexArray(m_render_ID);
	index_buffer->bind();

	m_index_buffer = index_buffer;
}
