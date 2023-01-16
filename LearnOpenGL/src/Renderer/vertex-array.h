#pragma once
#include <memory>
#include <vector>
#include "buffer.h"

class Vertex_Array
{
public:
	Vertex_Array();
	~Vertex_Array();

	void bind() const;
	void unbind() const;

	void add_vertex_buffer(const std::shared_ptr<Vertex_Buffer>& vertex_buffer);
	void set_index_buffer(const std::shared_ptr<Index_Buffer>& index_buffer);

	const std::vector<std::shared_ptr<Vertex_Buffer>>& get_vertex_buffers() const { return m_vertex_buffers; }
	const std::shared_ptr<Index_Buffer>& get_index_buffer() const { return m_index_buffer; }
private:
	std::vector<std::shared_ptr<Vertex_Buffer>> m_vertex_buffers;
	std::shared_ptr<Index_Buffer> m_index_buffer;
	uint32_t m_render_ID;
};