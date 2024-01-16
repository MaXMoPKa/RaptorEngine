#include "ecs/i_system.hpp"

using namespace raptor_engine::ecs;

i_system::i_system(system_priority priority_, f32 update_interval_ms_) 
	: time_since_last_update {0}
	, priority {priority_} 
	, update_interval {update_interval_ms_}
	, is_enabled {true},
	  is_need_update {}
	, reserved {}
{

}