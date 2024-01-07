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

	renderable_object();

	renderable_object(geometry_object_sptr				geometry_object_,
					  shader_program_sptr				shader_program_,
					  structs::draw_config_sptr			draw_config_,
					  std::vector<texture_program_sptr> textures_ = std::vector<texture_program_sptr> {});

	~renderable_object();

public:

	const std::vector<texture_program_sptr>& get_textures() const;

	const geometry_object_sptr get_geometry_object() const;

	const shader_program_sptr get_shader_program() const;

	const structs::draw_config_sptr get_draw_config() const;

private:
	class renderable_object_pimpl;
	std::unique_ptr<renderable_object_pimpl> pimpl;
};

using renderable_object_uptr = std::unique_ptr<renderable_object>;
using renderable_object_sptr = std::shared_ptr<renderable_object>;
using renderable_object_wptr = std::weak_ptr<renderable_object>;

} // namespace render
} // namespace raptor_engine