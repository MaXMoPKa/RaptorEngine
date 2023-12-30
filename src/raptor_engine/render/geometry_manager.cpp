#include "render/geometry_manager.hpp"

using namespace raptor_engine::render;

class geometry_manager::geometry_manager_pimpl
{
public:
	geometry_manager_pimpl() 
		: geometry_objects {} 
	{ }

	geometry_manager_pimpl(const std::vector<geometry_object_sptr>& geometry_objects_) 
		: geometry_objects {geometry_objects_}
	{ }

public:
	void create(const std::vector<geometry_object_sptr>& geometry_objects_)
	{
		geometry_manager_pimpl tmp {geometry_objects_};
		this->swap(tmp);
	}

	void swap(geometry_manager_pimpl& pimpl_)
	{
		if (this == &pimpl_) {
			return;
		}

		std::swap(this->geometry_objects, pimpl_.geometry_objects);
	}

	void reset() noexcept
	{
		geometry_manager_pimpl tmp {};
		this->swap(tmp);
	}

public:

	geometry_object_sptr add_geometry(const std::vector<float>&		   vertices_,
									  const std::vector<unsigned int>& indices_)
	{
		vertex_array_object_sptr vao = std::make_shared<vertex_array_object>();
		vao->generate_array();

		vertex_attribute_pointer vertex_attrib_pointer_0 {vertices_.size() * sizeof(float),
														  vertices_.data(),
														  GL_STATIC_DRAW,
														  0,
														  3,
														  GL_FLOAT,
														  GL_FALSE,
														  6 * sizeof(float),
														  (void*)0};

		vertex_attribute_pointer vertex_attrib_pointer_1 {vertices_.size() * sizeof(float),
														  vertices_.data(),
														  GL_STATIC_DRAW,
														  1,
														  3,
														  GL_FLOAT,
														  GL_FALSE,
														  6 * sizeof(float),
														  (void*)(3 * sizeof(float))};

		std::vector<vertex_attribute_pointer> vertex_attribute_pointers {vertex_attrib_pointer_0,
																		 vertex_attrib_pointer_1};

		vertex_buffer_object_data_sptr vbo_data =
			std::make_shared<vertex_buffer_object_data>(vertex_attribute_pointers);
		vertex_buffer_object_sptr vbo = std::make_shared<vertex_buffer_object>(vbo_data);
		vbo->generate_buffer();

		element_buffer_object_data_sptr ebo_data =
			std::make_shared<element_buffer_object_data>(indices_.size() * sizeof(unsigned int),
														 indices_.data(),
														 GL_STATIC_DRAW);

		element_buffer_object_sptr ebo = std::make_shared<element_buffer_object>(ebo_data);
		ebo->generate_buffer();

		// initialization
		vao->use();
		vbo->set_buffer_data();
		ebo->set_buffer_data();
		vbo->set_attrib_pointers();

		geometry_objects.emplace_back(std::make_shared<geometry_object>(vao, vbo, ebo));
	     
		return geometry_objects.back();
	}

public:
	const std::vector<geometry_object_sptr>& get_geometry_objects() const noexcept
	{
		return this->geometry_objects;
	}

	const geometry_object_sptr& get_geometry_object(std::size_t index_) const
	{
		return geometry_objects[index_];
	}

private:
	std::vector<geometry_object_sptr> geometry_objects;
};

geometry_manager::geometry_manager() 
	: pimpl {std::make_unique<geometry_manager_pimpl>()}
{ }

geometry_manager::geometry_manager(const std::vector<geometry_object_sptr>& geometry_objects_)
	: pimpl {std::make_unique<geometry_manager_pimpl>(geometry_objects_)}
{ }

geometry_manager::geometry_manager(geometry_manager&& object_) noexcept = default;

geometry_manager& geometry_manager::operator=(geometry_manager&& object_) noexcept = default;

geometry_manager::~geometry_manager() = default;

void geometry_manager::create(const std::vector<geometry_object_sptr>& geometry_objects_)
{
	this->pimpl->create(geometry_objects_);
}

void geometry_manager::swap(geometry_manager& manager_) 
{
	this->pimpl.swap(manager_.pimpl);
}

void geometry_manager::reset() noexcept 
{
	this->pimpl->reset();
}

geometry_object_sptr geometry_manager::add_geometry(const std::vector<float>&		 vertices_,
									                const std::vector<unsigned int>& indices_)
{
	return this->pimpl->add_geometry(vertices_, indices_);
}

const std::vector<geometry_object_sptr>& geometry_manager::get_geometry_objects() const noexcept
{
	return this->pimpl->get_geometry_objects();
}

const geometry_object_sptr& geometry_manager::get_geometry_object(std::size_t index_) const
{
	return this->pimpl->get_geometry_object(index_);
}