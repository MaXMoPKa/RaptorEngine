#include "ecs/event/i_event.hpp"

using namespace raptor_engine::ecs::event;

IEvent::IEvent(EventTypeId id_) 
	: id {id_} 
{

}

IEvent::~IEvent() = default;