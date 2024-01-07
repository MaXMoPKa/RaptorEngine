#pragma once

#include "defines.hpp"

#include <utility>
#include <memory>
#include <vector>

namespace raptor_engine {
namespace render {

struct vertex_attribute_pointer
{
	u32         size;
	const void*	data;
	u32         usage;

	u32         attrib_pointer_index;
	u32         attrib_pointer_size;
	u32         attrib_pointer_type;
	u32         attrib_pointer_normalized;
	i32			attrib_pointer_stride;
	const void*	attrib_pointer_pointer;
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

	vertex_buffer_object();

	vertex_buffer_object(const vertex_buffer_object_data_sptr& vbo_data_);

	vertex_buffer_object(vertex_buffer_object&& vbo) noexcept;
	vertex_buffer_object& operator=(vertex_buffer_object&& vbo) noexcept;

	vertex_buffer_object(const vertex_buffer_object&)			 = delete;
	vertex_buffer_object& operator=(const vertex_buffer_object&) = delete;

	~vertex_buffer_object();

public:
	void create(const vertex_buffer_object_data_sptr& vbo_data_);

	void swap(vertex_buffer_object& vbo_) noexcept;

	void reset() noexcept;

public:
	void generate_buffer();

	void set_buffer_data();

	void set_attrib_pointers();

public:
	u32 get_id() const;

private:
	class vertex_buffer_object_pimpl;
	std::unique_ptr<vertex_buffer_object_pimpl> pimpl;
};

using vertex_buffer_object_uptr = std::unique_ptr<vertex_buffer_object>;
using vertex_buffer_object_sptr = std::shared_ptr<vertex_buffer_object>;
using vertex_buffer_object_wptr = std::weak_ptr<vertex_buffer_object>;

}
}