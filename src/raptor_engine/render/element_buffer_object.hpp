#pragma once

#include "glad/glad.h"

#include "defines.hpp"

#include <utility>
#include <memory>

namespace raptor_engine {
namespace structs
{

struct element_buffer_object_data
{
public:

	element_buffer_object_data()
		: size {}, data {}, usage {}
	{ }

	element_buffer_object_data(u32	       size,
							   const void* data,
							   u32         usage)
		: size {size}, data {data}, usage {usage}
	{ }

	~element_buffer_object_data() = default;

public:
	u32 get_size() const
	{
		return this->size;
	}

	const void* get_data() const
	{
		return this->data;
	}

	u32 get_usage() const
	{
		return this->usage;
	}

private:
	u32         size;
	const void* data;
	u32         usage;
};

using element_buffer_object_data_uptr = std::unique_ptr<element_buffer_object_data>;
using element_buffer_object_data_sptr = std::shared_ptr<element_buffer_object_data>;
using element_buffer_object_data_wptr = std::weak_ptr<element_buffer_object_data>;

}

namespace render {

class element_buffer_object
{
public:

	element_buffer_object();

	element_buffer_object(const structs::element_buffer_object_data_sptr& ebo_data_);

	element_buffer_object(element_buffer_object&& ebo_) noexcept;
	element_buffer_object& operator=(element_buffer_object&& ebo_) noexcept;

	element_buffer_object(const element_buffer_object&)			   = delete;
	element_buffer_object& operator=(const element_buffer_object&) = delete;

	~element_buffer_object();

public:
	void create(const structs::element_buffer_object_data_sptr& ebo_data_);

	void swap(element_buffer_object& ebo_) noexcept;

	void reset() noexcept;

public:
	void generate_buffer();

	void set_buffer_data();

public:
	u32 get_id() const;

private:
	class element_buffer_object_pimpl;
	std::unique_ptr<element_buffer_object_pimpl> pimpl;
};

using element_buffer_object_uptr = std::unique_ptr<element_buffer_object>;
using element_buffer_object_sptr = std::shared_ptr<element_buffer_object>;
using element_buffer_object_wptr = std::weak_ptr<element_buffer_object>;

}
}