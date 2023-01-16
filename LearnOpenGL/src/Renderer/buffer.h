#pragma once
#include <string>
#include <vector>
#include <iostream>

enum class Shader_Data_Type
{
	None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Bool, Mat3, Mat4
};

static uint32_t shader_data_type_size(Shader_Data_Type type)
{
	switch (type)
	{
	case Shader_Data_Type::Float:		return 4;
	case Shader_Data_Type::Float2:		return 4 * 2;
	case Shader_Data_Type::Float3:		return 4 * 3;
	case Shader_Data_Type::Float4:		return 4 * 4;
	case Shader_Data_Type::Mat3:		return 4 * 3 * 3;
	case Shader_Data_Type::Mat4:		return 4 * 4 * 4;
	case Shader_Data_Type::Int:			return 4;
	case Shader_Data_Type::Int2:		return 4 * 2;
	case Shader_Data_Type::Int3:		return 4 * 3;
	case Shader_Data_Type::Int4:		return 4 * 4;
	case Shader_Data_Type::Bool:		return 1;
	}
	std::cout << "unknown Shader_Data_Type!" << std::endl;
	return 0;
}

struct Buffer_Element
{
	std::string name;
	Shader_Data_Type type;
	uint32_t size;
	uint32_t offset;
	bool normalized;

	Buffer_Element() = default;
	Buffer_Element(Shader_Data_Type type, const std::string& name, bool normalized = false)
		:type(type), name(name), size(shader_data_type_size(type)), offset(0), normalized(normalized)
	{
	}

	uint32_t get_value_count() const		//how many value does this element have
	{
		switch (type)
		{
		case Shader_Data_Type::Float:		return 1;
		case Shader_Data_Type::Float2:		return 2;
		case Shader_Data_Type::Float3:		return 3;
		case Shader_Data_Type::Float4:		return 4;
		case Shader_Data_Type::Mat3:		return 3 * 3;
		case Shader_Data_Type::Mat4:		return 4 * 4;
		case Shader_Data_Type::Int:			return 1;
		case Shader_Data_Type::Int2:		return 2;
		case Shader_Data_Type::Int3:		return 3;
		case Shader_Data_Type::Int4:		return 4;
		case Shader_Data_Type::Bool:		return 1;
		}
		std::cout << "unknown Shader_Data_Type!" << std::endl;
		return 0;
	}
};

class Buffer_Layout
{
public:
	Buffer_Layout() = default;
	Buffer_Layout(const std::initializer_list<Buffer_Element>& elements)
		:m_elements(elements)
	{
		calculate_offset_and_stride();
	}

	inline uint32_t get_stride() const { return m_stride; }
	inline const std::vector<Buffer_Element>& get_elements() const { return m_elements; }

	std::vector<Buffer_Element>::const_iterator begin() const { return m_elements.begin(); }
	std::vector<Buffer_Element>::const_iterator end() const { return m_elements.end(); }

private:
	void calculate_offset_and_stride()
	{
		uint32_t offset = 0;
		m_stride = 0;
		for (auto& element : m_elements)
		{
			element.offset = offset; //calculate offset of element
			offset += element.size; //next offset is current offset add current element size
			m_stride += element.size; //stride is all elements' size
		}
	}

private:
	std::vector<Buffer_Element> m_elements;
	uint32_t m_stride = 0;
};

class Vertex_Buffer
{
public:
	Vertex_Buffer(float* vertices, uint32_t size);
	~Vertex_Buffer();

	void bind() const;
	void unbind() const;

	const Buffer_Layout& get_layout() const { return m_buffer_layout; }
	void set_layout(const Buffer_Layout& layout) { m_buffer_layout = layout; }


private:
	uint32_t m_render_ID;
	Buffer_Layout m_buffer_layout;
};


class Index_Buffer
{
public:
	Index_Buffer(uint32_t* indices, uint32_t count);
	~Index_Buffer();

	void bind() const;
	void unbind() const;

	uint32_t get_indices_count() const { return m_indices_count; }

private:
	uint32_t m_render_ID;
	uint32_t m_indices_count;
};