#pragma once

#include "glad/glad.h"

#include <utility>
#include <memory>

namespace raptor_engine {
namespace render {

struct vertex_buffer_object_data
{
public:

	vertex_buffer_object_data()
		: size {}, data {}, usage {}, attrib_pointer_index {},
		  attrib_pointer_size {}, attrib_pointer_type {},
		  attrib_pointer_normalized {}, attrib_pointer_stride {},
		  attrib_pointer_pointer {}
	{ }

	vertex_buffer_object_data(unsigned int size, const void* data, unsigned int usage,
							  unsigned int	attrib_pointer_index,
							  unsigned int	attrib_pointer_size,
							  unsigned int	attrib_pointer_type,
							  unsigned char attrib_pointer_normalized,
							  int			attrib_pointer_stride,
							  const void*	attrib_pointer_pointer)
		: size {size}, data {data}, usage {usage}, attrib_pointer_index {attrib_pointer_index},
		  attrib_pointer_size {attrib_pointer_size}, attrib_pointer_type {attrib_pointer_type},
		  attrib_pointer_normalized {attrib_pointer_normalized}, attrib_pointer_stride {attrib_pointer_stride},
		  attrib_pointer_pointer {attrib_pointer_pointer}
	{ }

	~vertex_buffer_object_data() = default;

public:
	unsigned int size;
	const void* data;
	unsigned int usage;

	unsigned int attrib_pointer_index;
	unsigned int attrib_pointer_size;
	unsigned int attrib_pointer_type;
	unsigned char attrib_pointer_normalized;
	int			  attrib_pointer_stride;
	const void*	  attrib_pointer_pointer;

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
		glBufferData(GL_ARRAY_BUFFER, vbo_data->size, vbo_data->data, vbo_data->usage);
	}

	void set_attrib_pointers()
	{
		glVertexAttribPointer(vbo_data->attrib_pointer_index,
							  vbo_data->attrib_pointer_size,
							  vbo_data->attrib_pointer_type,
							  vbo_data->attrib_pointer_normalized,
							  vbo_data->attrib_pointer_stride,
							  vbo_data->attrib_pointer_pointer);
		glEnableVertexAttribArray(0);
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