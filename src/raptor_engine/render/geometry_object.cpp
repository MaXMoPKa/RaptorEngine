#include "render/geometry_object.hpp"

using namespace raptor_engine::render;

class geometry_object::geometry_object_pimpl
{
public:
	geometry_object_pimpl() 
		: vao {nullptr}
		, vbo {nullptr}
		, ebo {nullptr}
	{ }

	geometry_object_pimpl(vertex_array_object_sptr	 vao_,
						  vertex_buffer_object_sptr	 vbo_,
						  element_buffer_object_sptr ebo_)
		: vao {vao_}
		, vbo {vbo_}
		, ebo {ebo_}
	{ }

	~geometry_object_pimpl() { }

public:
	void create(vertex_array_object_sptr vao_, vertex_buffer_object_sptr vbo_, element_buffer_object_sptr ebo_)
	{
		geometry_object_pimpl tmp {vao_, vbo_, ebo_};
		this->swap(tmp);
	}

	void swap(geometry_object_pimpl& pimpl_) noexcept
	{
		if (this == &pimpl_) {
			return;
		}

		std::swap(this->vao, pimpl_.vao);
		std::swap(this->vbo, pimpl_.vbo);
		std::swap(this->ebo, pimpl_.ebo);
	}

	void reset() noexcept
	{
		geometry_object_pimpl tmp {};
		this->swap(tmp);
	}

public:
	const vertex_array_object_sptr get_vao() const
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

geometry_object::geometry_object() 
	: pimpl {std::make_unique<geometry_object_pimpl>()} 
{ }

geometry_object::geometry_object(vertex_array_object_sptr   vao_,
								 vertex_buffer_object_sptr  vbo_,
								 element_buffer_object_sptr ebo_)
	: pimpl {std::make_unique<geometry_object_pimpl>(vao_, vbo_, ebo_)}
{ }

geometry_object::geometry_object(geometry_object&& object_) noexcept = default;

geometry_object& geometry_object::operator=(geometry_object&& object_) noexcept = default;

geometry_object::~geometry_object() = default;

void geometry_object::create(vertex_array_object_sptr   vao_,
							 vertex_buffer_object_sptr  vbo_,
							 element_buffer_object_sptr ebo_)
{
	this->pimpl->create(vao_, vbo_, ebo_);
}

void geometry_object::swap(geometry_object& object_) noexcept 
{
	this->pimpl.swap(object_.pimpl);
}

void geometry_object::reset() noexcept
{
	this->pimpl->reset();
}

const vertex_array_object_sptr geometry_object::get_vao() const
{
	return this->pimpl->get_vao();
}

const vertex_buffer_object_sptr geometry_object::get_vbo() const
{
	return this->pimpl->get_vbo();
}

const element_buffer_object_sptr geometry_object::get_ebo() const
{
	return this->pimpl->get_ebo();
}

void geometry_object::set_vao(vertex_array_object_sptr new_vao_) 
{
	this->pimpl->set_vao(new_vao_);
}

void geometry_object::set_vbo(vertex_buffer_object_sptr new_vbo_) 
{
	this->pimpl->set_vbo(new_vbo_);
}

void geometry_object::set_ebo(element_buffer_object_sptr new_ebo_) 
{
	this->pimpl->set_ebo(new_ebo_);
}