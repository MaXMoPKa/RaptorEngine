#include "structs/render_engine_data.hpp"

using namespace raptor_engine::structs;

render_engine_data::render_engine_data() : 
	  renderer_data {},
	  high_level_renderer {high_level_renderer_type::FORWARD_LDR_RENDERER}
{ }

render_engine_data::render_engine_data(
	const high_level_renderer_data_sptr& renderer_info,
	const high_level_renderer_type&	renderer_type)
	: renderer_data {renderer_info}, 
	  high_level_renderer {renderer_type}
{ }

render_engine_data::~render_engine_data() = default;