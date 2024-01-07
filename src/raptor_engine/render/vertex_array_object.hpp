#pragma once

#include "glad/glad.h"

#include <utility>
#include <memory>

#include "defines.hpp"

namespace raptor_engine {
namespace render {

class vertex_array_object
{
public:

	vertex_array_object();

	vertex_array_object(u32 vao_);

	vertex_array_object(vertex_array_object&& vao_) noexcept;
	vertex_array_object& operator=(vertex_array_object&& vao_) noexcept;

	vertex_array_object(const vertex_array_object&) = delete;
	vertex_array_object& operator=(const vertex_array_object&) = delete;

	~vertex_array_object();

public:
	void create();

	void swap(vertex_array_object& vao) noexcept;

	void reset() noexcept;

public:
	void generate_array();

	void use();

public:
	unsigned int get_id() const;

private:
	class vertex_array_object_pimpl;
	std::unique_ptr<vertex_array_object_pimpl> pimpl;
};

using vertex_array_object_uptr = std::unique_ptr<vertex_array_object>;
using vertex_array_object_sptr = std::shared_ptr<vertex_array_object>;
using vertex_array_object_wptr = std::weak_ptr<vertex_array_object>;

}
}