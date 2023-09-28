#include "structs/high_level_renderer_data.hpp"

using namespace raptor_engine::structs;

high_level_renderer_data::high_level_renderer_data()
	: window_ptr {}, 
	  low_level_render_api {low_level_render_api_type::OPENGL}
{ }

high_level_renderer_data::high_level_renderer_data(
	const sdl_window_sptr&			 sdl_window_ptr,
	const low_level_render_api_type& render_api)
	: window_ptr {sdl_window_ptr}, 
	  low_level_render_api {render_api}
{ }

high_level_renderer_data::~high_level_renderer_data() = default;