#include "ecs/event/i_event_listener.hpp"

using namespace raptor_engine::ecs::event;

IEventListener::~IEventListener()
{
	this->UnregisterAllEventCallbacks();
}

void IEventListener::UnregisterAllEventCallbacks()
{
	for (const IEventDelegateSptr& cb : this->GetRegisteredCallbacks())
	{
		Engine::GetInstance()->UnsubscribeEvent(cb);
	}

	this->GetRegisteredCallbacks().clear();
}