#pragma once

#include "glad/glad.h"

#include "defines.hpp"

#include <utility>
#include <memory>

namespace raptor_engine {
namespace render {

struct vertex_attribute_pointer
{
	unsigned int size;
	const void*	 data;
	unsigned int usage;

	unsigned int  attrib_pointer_index;
	unsigned int  attrib_pointer_size;
	unsigned int  attrib_pointer_type;
	unsigned char attrib_pointer_normalized;
	int			  attrib_pointer_stride;
	const void*	  attrib_pointer_pointer;
};

struct vertex_buffer_object_data
{
public:

	vertex_buffer_object_data() 
		: vertex_attribute_pointers {}
	{ }

	vertex_buffer_object_data(std::vector<vertex_attribute_pointer>& vertex_attribute_pointers_)
		: vertex_attribute_pointers {vertex_attribute_pointers_}
	{ }

	~vertex_buffer_object_data() = default;

public:
	std::vector<vertex_attribute_pointer> vertex_attribute_pointers;
};

using vertex_buffer_object_data_uptr = std::unique_ptr<vertex_buffer_object_data>;
using vertex_buffer_object_data_sptr = std::shared_ptr<vertex_buffer_object_data>;
using vertex_buffer_object_data_wptr = std::weak_ptr<vertex_buffer_object_data>;

class vertex_buffer_object
{
public:

	vertex_buffer_object() : vbo {}, vbo_data {} { }

	vertex_buffer_object(const vertex_buffer_object_data_sptr& vbo_data_) 
		: vbo {}, vbo_data {vbo_data_}
	{ }

	vertex_buffer_object(vertex_buffer_object&& vbo) noexcept			 = default;
	vertex_buffer_object& operator=(vertex_buffer_object&& vbo) noexcept = default;

	vertex_buffer_object(const vertex_buffer_object&)			= delete;
	vertex_buffer_object& operator=(const vertex_buffer_object&) = delete;

	~vertex_buffer_object()
	{
		if (vbo) 
		{
			glDeleteBuffers(1, &vbo);
		}
	}

public:

	void create(const vertex_buffer_object_data_sptr& vbo_data_)
	{
		vertex_buffer_object tmp {vbo_data_};
		this->swap(tmp);
	}

	void swap(vertex_buffer_object& vbo) noexcept
	{
		if (this == &vbo)
		{
			return;
		}

		std::swap(this->vbo, vbo.vbo);
	}

	void reset() noexcept
	{
		vertex_buffer_object tmp {};
		this->swap(tmp);
	}

public:
	void generate_buffer()
	{
		glGenBuffers(1, &vbo);
	}

	void set_buffer_data()
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER,
					 vbo_data->vertex_attribute_pointers[0].size,
					 vbo_data->vertex_attribute_pointers[0].data,
					 vbo_data->vertex_attribute_pointers[0].usage);
	}

	void set_attrib_pointers()
	{
		u64 vertex_attrib_index = 0;
		for (const auto& vbo_attrib_pointer : vbo_data->vertex_attribute_pointers) 
		{
			glVertexAttribPointer(vbo_attrib_pointer.attrib_pointer_index,
								  vbo_attrib_pointer.attrib_pointer_size,
								  vbo_attrib_pointer.attrib_pointer_type,
								  vbo_attrib_pointer.attrib_pointer_normalized,
								  vbo_attrib_pointer.attrib_pointer_stride,
								  vbo_attrib_pointer.attrib_pointer_pointer);
			glEnableVertexAttribArray(vertex_attrib_index++);
		}
	}

public:
	unsigned int get_id() const
	{
		return this->vbo;
	}

private:
	unsigned int vbo;
	vertex_buffer_object_data_sptr vbo_data;
};

using vertex_buffer_object_uptr = std::unique_ptr<vertex_buffer_object>;
using vertex_buffer_object_sptr = std::shared_ptr<vertex_buffer_object>;
using vertex_buffer_object_wptr = std::weak_ptr<vertex_buffer_object>;

}
}