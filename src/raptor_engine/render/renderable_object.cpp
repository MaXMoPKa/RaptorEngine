#include "render/renderable_object.hpp"

using namespace raptor_engine::render;

class renderable_object::renderable_object_pimpl
{
public:

	renderable_object_pimpl() 
		: textures {}
		, geometry_object {}
		, shader_program {}
		, draw_config {} 
	{ }

	renderable_object_pimpl(geometry_object_sptr			  geometry_object_,
					        shader_program_sptr				  shader_program_,
					        structs::draw_config_sptr	      draw_config_,
					        std::vector<texture_program_sptr> textures_ = std::vector<texture_program_sptr> {})
		: textures {textures_}
		, geometry_object {geometry_object_}
		, shader_program {shader_program_}
		, draw_config {draw_config_}
	{ }

	~renderable_object_pimpl() = default;

public:
	std::vector<texture_program_sptr> get_textures() const
	{
		return this->textures;
	}

	geometry_object_sptr get_geometry_object() const
	{
		return geometry_object;
	}

	shader_program_sptr get_shader_program() const
	{
		return shader_program;
	}

	structs::draw_config_sptr get_draw_config() const
	{
		return draw_config;
	}

private:
	std::vector<texture_program_sptr> textures;
	geometry_object_sptr			  geometry_object;
	shader_program_sptr				  shader_program;
	structs::draw_config_sptr		  draw_config;
};