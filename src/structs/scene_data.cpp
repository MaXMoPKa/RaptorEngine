#include "structs/scene_data.hpp"

using namespace raptor_engine;
using namespace raptor_engine::structs;

scene_data::scene_data() 
	: objects {}
{

}

scene_data::scene_data(const render_objects& render_objects_)
	: objects{ render_objects_ }
{

}

scene_data::~scene_data() = default;