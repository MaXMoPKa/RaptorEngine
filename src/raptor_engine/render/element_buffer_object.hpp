#pragma once

#include "glad/glad.h"

#include <utility>
#include <memory>

namespace raptor_engine {
namespace render {

struct element_buffer_object_data
{
public:

	element_buffer_object_data()
		: size {}, data {}, usage {}
	{ }

	element_buffer_object_data(unsigned int	 size,
							   const void*	 data,
							   unsigned int	 usage)
		: size {size}, data {data}, usage {usage}
	{ }

	~element_buffer_object_data() = default;

public:
	unsigned int size;
	const void* data;
	unsigned int usage;
};

using element_buffer_object_data_uptr = std::unique_ptr<element_buffer_object_data>;
using element_buffer_object_data_sptr = std::shared_ptr<element_buffer_object_data>;
using element_buffer_object_data_wptr = std::weak_ptr<element_buffer_object_data>;

class element_buffer_object
{
public:

	element_buffer_object() : ebo {}, ebo_data {} { }

	element_buffer_object(const element_buffer_object_data_sptr& ebo_data_) 
		: ebo {}, ebo_data {ebo_data_}
	{ }

	element_buffer_object(element_buffer_object&& ebo_) noexcept			= default;
	element_buffer_object& operator=(element_buffer_object&& ebo_) noexcept = default;

	element_buffer_object(const element_buffer_object&)			   = delete;
	element_buffer_object& operator=(const element_buffer_object&) = delete;

	~element_buffer_object()
	{
		glDeleteBuffers(1, &ebo);
	}

public:

	void create(const element_buffer_object_data_sptr& ebo_data_)
	{
		element_buffer_object tmp {ebo_data_};
		this->swap(tmp);
	}

	void swap(element_buffer_object& ebo_) noexcept
	{
		if (this == &ebo_)
		{
			return;
		}

		std::swap(this->ebo, ebo_.ebo);
	}

	void reset() noexcept
	{
		element_buffer_object tmp {};
		this->swap(tmp);
	}

public:

	void generate_buffer()
	{
		glGenBuffers(1, &ebo);
	}

	void set_buffer_data()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_data->size, ebo_data->data, ebo_data->usage);
	}

private:
	unsigned int ebo;

	element_buffer_object_data_sptr ebo_data;
};

using element_buffer_object_uptr = std::unique_ptr<element_buffer_object>;
using element_buffer_object_sptr = std::shared_ptr<element_buffer_object>;
using element_buffer_object_wptr = std::weak_ptr<element_buffer_object>;

}
}