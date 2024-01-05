# include "render/element_buffer_object.hpp"

using namespace raptor_engine::render;
using namespace raptor_engine::structs;

class element_buffer_object::element_buffer_object_pimpl
{
public:
	element_buffer_object_pimpl()
		: id {0}, data {}
	{

	}

	element_buffer_object_pimpl(const element_buffer_object_data_sptr& data_) 
		: id {0}, data {data_} 
	{

	}

	~element_buffer_object_pimpl()
	{
		if (id)
		{
			glDeleteBuffers(1, &(this->id));
		}
	}

public:

	void create(const element_buffer_object_data_sptr& data_)
	{
		element_buffer_object_pimpl tmp {data_};
		this->swap(tmp);
	}

	void swap(element_buffer_object_pimpl& ebo_) noexcept
	{
		if (this == &ebo_)
		{
			return;
		}

		std::swap(this->id, ebo_.id);
		std::swap(this->data, ebo_.data);
	}

	void reset() noexcept
	{
		element_buffer_object_pimpl tmp {};
		this->swap(tmp);
	}

public:
	void generate_buffer()
	{
		glGenBuffers(1, &(this->id));
	}

	void set_buffer_data()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->data->get_size(), this->data->get_data(), this->data->get_usage());
	}

public:

	u32 get_id() const
	{
		return this->id;
	}

private:
	u32 id;

	element_buffer_object_data_sptr data;
};

element_buffer_object::element_buffer_object() 
	: pimpl {std::make_unique<element_buffer_object_pimpl>()} 
{

}

element_buffer_object::element_buffer_object(const element_buffer_object_data_sptr& data_) 
	: pimpl {std::make_unique<element_buffer_object_pimpl>(data_)}
{

}

element_buffer_object::element_buffer_object(element_buffer_object&& ebo_) noexcept = default;
element_buffer_object& element_buffer_object::operator=(element_buffer_object&& ebo_) noexcept = default;

element_buffer_object::~element_buffer_object() = default;

void element_buffer_object::create(const element_buffer_object_data_sptr& data_)
{
	this->pimpl->create(data_);
}

void element_buffer_object::swap(element_buffer_object& ebo_) noexcept 
{
	this->pimpl.swap(ebo_.pimpl);
}

void element_buffer_object::reset() noexcept
{
	this->pimpl->reset();
}

void element_buffer_object::generate_buffer()
{
	this->pimpl->generate_buffer();
}

void element_buffer_object::set_buffer_data()
{
	this->pimpl->set_buffer_data();
}

u32 element_buffer_object::get_id() const
{
	return this->pimpl->get_id();
}