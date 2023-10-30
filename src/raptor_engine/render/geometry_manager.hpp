#pragma once

#include <vector>

#include "render/vertex_array_object.hpp"
#include "render/vertex_buffer_object.hpp"
#include "render/element_buffer_object.hpp"

namespace raptor_engine
{
namespace render
{

struct geometry_object
{
public:

	geometry_object()
		: vao{ nullptr }
		, vbo{ nullptr }
		, ebo{ nullptr }
	{ }

	geometry_object(vertex_array_object_sptr vao_,
					vertex_buffer_object_sptr vbo_,
					element_buffer_object_sptr ebo_)
		: vao {vao_}
		, vbo {vbo_}
		, ebo {ebo_}
	{ }

	geometry_object(geometry_object&& object_) noexcept			   = default;
	geometry_object& operator=(geometry_object&& object_) noexcept = default;

	geometry_object(const geometry_object&)			 = delete;
	geometry_object& operator=(const geometry_object&) = delete;

	~geometry_object() = default;

public:
	void create(vertex_array_object_sptr vao_, vertex_buffer_object_sptr vbo_, element_buffer_object_sptr ebo_)
	{
		geometry_object tmp {vao_, vbo_, ebo_};
		this->swap(tmp);
	}

	void swap(geometry_object& object_) noexcept
	{
		if (this == &object_)
		{
			return;
		}

		std::swap(this->vao, object_.vao);
		std::swap(this->vbo, object_.vbo);
		std::swap(this->ebo, object_.ebo);
	}

	void reset() noexcept
	{
		geometry_object tmp {};
		this->swap(tmp);
	}

public:
	const vertex_array_object_sptr& get_vao() const
	{
		return this->vao;
	}

	const vertex_buffer_object_sptr get_vbo() const
	{
		return this->vbo;
	}

	const element_buffer_object_sptr get_ebo() const
	{
		return this->ebo;
	}

public:
	void set_vao(vertex_array_object_sptr new_vao_)
	{
		this->vao = new_vao_;
	}

	void set_vbo(vertex_buffer_object_sptr new_vbo_)
	{
		this->vbo = new_vbo_;
	}

	void set_ebo(element_buffer_object_sptr new_ebo_)
	{
		this->ebo = new_ebo_;
	}

private:
	vertex_array_object_sptr   vao;
	vertex_buffer_object_sptr  vbo;
	element_buffer_object_sptr ebo;
};

class geometry_manager
{
public:

	geometry_manager() : vao {nullptr}, vbo {0}, ebo {0} { }

public:

	void add_geometry(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) 
	{ 
		vao = std::make_shared<vertex_array_object>();
		vao->generate_array();

		auto vbo_data = std::make_shared<vertex_buffer_object_data>(vertices.size() * sizeof(float),
																	vertices.data(),
																	GL_STATIC_DRAW,
																	0,
																	3,
																	GL_FLOAT,
																	GL_FALSE,
																	3 * sizeof(float),
																	(void*)0);
		vbo = std::make_shared<vertex_buffer_object>(vbo_data);
		vbo->generate_buffer();

		auto ebo_data = std::make_shared<element_buffer_object_data>(indices.size() * sizeof(unsigned int),
																	 indices.data(),
																	 GL_STATIC_DRAW);

		ebo = std::make_shared<element_buffer_object>(ebo_data);
		ebo->generate_buffer();

		// initialization
		vao->use();
		vbo->set_buffer_data();
		ebo->set_buffer_data();
		vbo->set_attrib_pointers();
	}

public:

	vertex_array_object_sptr   vao;
	vertex_buffer_object_sptr  vbo;
	element_buffer_object_sptr ebo;
};

} // namespace render
} // namespace raptor_engine