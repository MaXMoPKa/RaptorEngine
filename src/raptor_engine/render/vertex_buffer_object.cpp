#include "render/vertex_buffer_object.hpp"

#include "glad/glad.h"

using namespace raptor_engine::render;

class vertex_buffer_object::vertex_buffer_object_pimpl
{
public:
	vertex_buffer_object_pimpl() 
		: id {}
		, vbo_data {} 
	{

	}

	vertex_buffer_object_pimpl(const vertex_buffer_object_data_sptr& vbo_data_) 
		: id {}
		, vbo_data {vbo_data_} 
	{

	}

	~vertex_buffer_object_pimpl()
	{
		if (this->id) {
			glDeleteBuffers(1, &(this->id));
		}
	}

public:
	void create(const vertex_buffer_object_data_sptr& vbo_data_)
	{
		vertex_buffer_object_pimpl tmp {vbo_data_};
		this->swap(tmp);
	}

	void swap(vertex_buffer_object_pimpl& vbo_) noexcept
	{
		if (this == &vbo_) {
			return;
		}

		std::swap(this->id, vbo_.id);
	}

	void reset() noexcept
	{
		vertex_buffer_object_pimpl tmp {};
		this->swap(tmp);
	}

public:
	void generate_buffer()
	{
		glGenBuffers(1, &(this->id));
	}

	void set_buffer_data()
	{
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
		glBufferData(GL_ARRAY_BUFFER,
					 vbo_data->vertex_attribute_pointers[0].size,
					 vbo_data->vertex_attribute_pointers[0].data,
					 vbo_data->vertex_attribute_pointers[0].usage);
	}

	void set_attrib_pointers()
	{
		u64 vertex_attrib_index = 0;
		for (const auto& vbo_attrib_pointer : vbo_data->vertex_attribute_pointers) {
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
	u32 get_id() const
	{
		return this->id;
	}

private:
	u32                            id;
	vertex_buffer_object_data_sptr vbo_data;
};

vertex_buffer_object::vertex_buffer_object() 
	: pimpl {std::make_unique<vertex_buffer_object_pimpl>()} 
{

}

vertex_buffer_object::vertex_buffer_object(const vertex_buffer_object_data_sptr& vbo_data_) 
	: pimpl {std::make_unique<vertex_buffer_object_pimpl>(vbo_data_)}
{ 

}

vertex_buffer_object::vertex_buffer_object(vertex_buffer_object&& vbo) noexcept            = default;
vertex_buffer_object& vertex_buffer_object::operator=(vertex_buffer_object&& vbo) noexcept = default;

vertex_buffer_object::~vertex_buffer_object() = default;

void vertex_buffer_object::create(const vertex_buffer_object_data_sptr& vbo_data_)
{
	this->pimpl->create(vbo_data_);
}

void vertex_buffer_object::swap(vertex_buffer_object& vbo_) noexcept
{
	this->pimpl.swap(vbo_.pimpl);
}

void vertex_buffer_object::reset() noexcept
{
	this->pimpl->reset();
}

void vertex_buffer_object::generate_buffer()
{
	this->pimpl->generate_buffer();
}

void vertex_buffer_object::set_buffer_data()
{
	this->pimpl->set_buffer_data();
}

void vertex_buffer_object::set_attrib_pointers()
{
	this->pimpl->set_attrib_pointers();
}

u32 vertex_buffer_object::get_id() const 
{
	return this->pimpl->get_id();
}