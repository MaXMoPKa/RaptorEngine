#include "structs/high_level_renderer_data.hpp"

using namespace raptor_engine::structs;

high_level_renderer_data::high_level_renderer_data()
	: window_ptr {}, 
	  render_api_type {low_level_render_api_type::OPENGL}
{ }

high_level_renderer_data::high_level_renderer_data(
	const sdl_window_sptr&			 sdl_window_ptr_,
	const low_level_render_api_type& render_api_type_)
	: window_ptr {sdl_window_ptr_}, 
	  render_api_type {render_api_type_}
{ }

high_level_renderer_data::~high_level_renderer_data() = default;

const sdl_window_sptr high_level_renderer_data::get_window() const
{
	return this->window_ptr;
}

low_level_render_api_type high_level_renderer_data::get_render_api_type() const
{
	return this->render_api_type;
}