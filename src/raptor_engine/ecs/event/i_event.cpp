#include "ecs/event/i_event.hpp"

#include "core/engine.hpp"

#include <cassert>

using namespace raptor_engine::ecs::event;

IEvent::IEvent(EventTypeId id_) 
	: id {id_} 
{
	assert(Engine::GetInstance() != nullptr && "Engin not initialized!");
	this->timeCreated = Engine::GetInstance()->GetTimer()->GetTimeStamp();
}

IEvent::~IEvent() = default;