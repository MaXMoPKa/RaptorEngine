#include "render/draw_config.hpp"

using namespace raptor_engine::structs;

class draw_config::draw_config_pimpl
{
public:

	draw_config_pimpl()
		: mode{ GL_TRIANGLES }
		, count{ 0 }
		, type{ GL_UNSIGNED_INT }
		, indices{ nullptr }
		, wireframe{ false }
	{

	}

	draw_config_pimpl(i32 mode_, i32 count_, i32 type_, void* indices_, bool wireframe_) 
		: mode {mode_}
		, count {count_}
		, type {type_}
		, indices {indices_}
		, wireframe {wireframe_}
	{

	}

	~draw_config_pimpl() = default;

public:
	i32 get_mode() const
	{
		return this->mode;
	}

	i32 get_count() const
	{
		return this->count;
	}

	i32 get_type() const
	{
		return this->type;
	}

	void* get_indices() const
	{
		return this->indices;
	}

	bool get_wireframe() const
	{
		return this->wireframe;
	}

private:
	i32	  mode;
	i32	  count;
	i32	  type;
	void* indices;
	bool  wireframe;
};

draw_config::draw_config() 
	: pimpl {std::make_unique<draw_config_pimpl>()} 
{

}

draw_config::draw_config(i32 mode_, i32 count_, i32 type_, void* indices_, bool wireframe_)
	: pimpl {std::make_unique<draw_config_pimpl>(mode_, count_, type_, indices_, wireframe_)}
{

}

draw_config::~draw_config() = default;

i32 draw_config::get_mode() const
{
	return this->pimpl->get_mode();
}

i32 draw_config::get_count() const
{
	return this->pimpl->get_count();
}

i32 draw_config::get_type() const
{
	return this->pimpl->get_type();
}

void* draw_config::get_indices() const
{
	return this->pimpl->get_indices();
}

bool draw_config::get_wireframe() const
{
	return this->pimpl->get_wireframe();
}