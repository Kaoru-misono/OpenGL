#include "buffer.h"

#include <glad/glad.h>

//===========================================================================================
//-----------------------------------vertex buffer-------------------------------------------
//===========================================================================================
Vertex_Buffer::Vertex_Buffer(float* vertices, uint32_t size)
{
	glGenBuffers(1, &m_render_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_render_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

Vertex_Buffer::~Vertex_Buffer()
{
	glDeleteBuffers(1, &m_render_ID);
}

void Vertex_Buffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_render_ID);
}

void Vertex_Buffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Index_Buffer::Index_Buffer(uint32_t* indices, uint32_t count) : m_indices_count(count)
{
	glGenBuffers(1, &m_render_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_render_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

Index_Buffer::~Index_Buffer()
{
	glDeleteBuffers(1, &m_render_ID);
}

void Index_Buffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_render_ID);
}

void Index_Buffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
