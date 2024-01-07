#include "structs/init_render_engine_data.hpp"

using namespace raptor_engine::structs;

init_render_engine_data::init_render_engine_data() 
	: renderer_info {}
	, renderer_type {high_level_renderer_type::FORWARD_LDR_RENDERER}
	, scene_info {}
{ }

init_render_engine_data::init_render_engine_data(
	const high_level_renderer_data_sptr& renderer_info_,
	const high_level_renderer_type&	renderer_type_,
	const scene_data_sptr& scene_info_)
	: renderer_info {renderer_info_}
	, renderer_type {renderer_type_}
	, scene_info {scene_info_}
{ }

init_render_engine_data::~init_render_engine_data() = default;

const high_level_renderer_data_sptr init_render_engine_data::get_renderer_info() const
{
	return this->renderer_info;
}

high_level_renderer_type init_render_engine_data::get_renderer_type() const
{
	return this->renderer_type;
}

const scene_data_sptr init_render_engine_data::get_scene_info() const
{
	return this->scene_info;
}