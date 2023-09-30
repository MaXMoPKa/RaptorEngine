#include "structs/scene_data.hpp"

using namespace raptor_engine;
using namespace raptor_engine::structs;
using namespace raptor_engine::render;

scene_data::scene_data() 
	: objects {}, draw_config {}
	{

}

scene_data::scene_data(const render_objects& render_objects_, const draw_config_sptr& draw_config_)
	: objects {render_objects_}, draw_config {draw_config_}
{

}

scene_data::~scene_data() = default;