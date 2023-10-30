#pragma once

#include "glad/glad.h"

#include <utility>
#include <memory>

namespace raptor_engine {
namespace render {

class vertex_array_object
{
public:

	vertex_array_object() : vao {}
	{ }

	vertex_array_object(vertex_array_object&& vao) noexcept = default;
	vertex_array_object& operator=(vertex_array_object&& vao) noexcept = default;

	vertex_array_object(const vertex_array_object&) = delete;
	vertex_array_object& operator=(const vertex_array_object&) = delete;

	~vertex_array_object()
	{
		if (vao) 
		{
			glDeleteVertexArrays(1, &vao);
		}
	}

public:
	void create()
	{
		vertex_array_object tmp {};
		this->swap(tmp);
	}

	void swap(vertex_array_object& vao) noexcept
	{
		if (this == &vao)
		{
			return;
		}

		std::swap(this->vao, vao.vao);
	}

	void reset() noexcept
	{
		vertex_array_object tmp {};
		this->swap(tmp);
	}

public:
	void generate_array()
	{
		glGenVertexArrays(1, &vao);
	}

	void use()
	{
		glBindVertexArray(vao);
	}

public:
	unsigned int get_id() const
	{
		return this->vao;
	}

private:
	unsigned int vao;
};

using vertex_array_object_uptr = std::unique_ptr<vertex_array_object>;
using vertex_array_object_sptr = std::shared_ptr<vertex_array_object>;
using vertex_array_object_wptr = std::weak_ptr<vertex_array_object>;

}
}