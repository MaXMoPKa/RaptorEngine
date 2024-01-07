#include "render/vertex_array_object.hpp"

#include "glad/glad.h"

using namespace raptor_engine::render;

class vertex_array_object::vertex_array_object_pimpl
{
public:
	vertex_array_object_pimpl() 
		: vao {0} 
	{ 
	
	}

	vertex_array_object_pimpl(u32 vao_) 
		: vao {vao_} 
	{
	
	}

	~vertex_array_object_pimpl()
	{
		if (this->vao) {
			glDeleteVertexArrays(1, &(this->vao));
		}
	}

public:
	void create()
	{
		vertex_array_object_pimpl tmp {};
		this->swap(tmp);
	}

	void swap(vertex_array_object_pimpl& vao_) noexcept
	{
		if (this == &vao_) {
			return;
		}

		std::swap(this->vao, vao_.vao);
	}

	void reset() noexcept
	{
		vertex_array_object_pimpl tmp {};
		this->swap(tmp);
	}

public:

	void generate_array()
	{
		glGenVertexArrays(1, &(this->vao));
	}

	void use()
	{
		glBindVertexArray(this->vao);
	}

public:

	unsigned int get_id() const
	{
		return this->vao;
	}

private:
	u32 vao;
};

vertex_array_object::vertex_array_object() 
	: pimpl {std::make_unique<vertex_array_object_pimpl>()} 
{

}

vertex_array_object::vertex_array_object(u32 vao_) 
	: pimpl {std::make_unique<vertex_array_object_pimpl>(vao_)} 
{

}

vertex_array_object::vertex_array_object(vertex_array_object&& vao_) noexcept            = default;
vertex_array_object& vertex_array_object::operator=(vertex_array_object&& vao_) noexcept = default;

vertex_array_object::~vertex_array_object() = default;

void vertex_array_object::create()
{
	this->pimpl->create();
}

void vertex_array_object::swap(vertex_array_object& vao_) noexcept
{
	this->pimpl.swap(vao_.pimpl);
}

void vertex_array_object::reset() noexcept
{
	this->pimpl->reset();
}

void vertex_array_object::generate_array()
{
	this->pimpl->generate_array();
}

void vertex_array_object::use()
{
	this->pimpl->use();
}

u32 vertex_array_object::get_id() const
{
	return this->pimpl->get_id();
}