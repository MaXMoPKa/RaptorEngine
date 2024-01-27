#include "ecs/system/i_system.hpp"

using namespace raptor_engine::ecs;

ISystem::ISystem(SystemPriority priority_, f32 updateIntervalMs_) 
	: timeSinceLastUpdate {0}
	, priority {priority_} 
	, updateInterval {updateIntervalMs_}
	, isEnabled {true},
	  isNeedUpdate {}
	, reserved {}
{

}