#pragma once

#include "defines.hpp"

#include "render/texture_program.hpp"
#include "render/geometry_object.hpp"
#include "render/shader_program.hpp"
#include "render/draw_config.hpp"

namespace raptor_engine
{
namespace render
{

class renderable_object
{
public:
	renderable_object() 
		: textures {}
		, geometry_object {}
		, shader_program {}
		, draw_config {}
	{

	}

	renderable_object(geometry_object_sptr geometry_object_,
		              shader_program_sptr shader_program_,
		              draw_config_sptr draw_config_,
					  std::vector<texture_program_sptr> textures_ = std::vector<texture_program_sptr> {}) 
		: textures(textures_)
		, geometry_object {geometry_object_}
		, shader_program {shader_program_}
		, draw_config {draw_config_}
	{

	}

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

	draw_config_sptr get_draw_config() const
	{
		return draw_config;
	}

private:
	std::vector<texture_program_sptr> textures;
	geometry_object_sptr geometry_object;
	shader_program_sptr	 shader_program;
	draw_config_sptr	 draw_config;
};

using renderable_object_uptr = std::unique_ptr<renderable_object>;
using renderable_object_sptr = std::shared_ptr<renderable_object>;
using renderable_object_wptr = std::weak_ptr<renderable_object>;

} // namespace render
} // namespace raptor_engine