#include "ecs/event/i_event_listener.hpp"

namespace raptor_engine
{
namespace ecs
{
namespace event
{

IEventListener::~IEventListener()
{
	this->UnregisterAllEventCallbacks();
}

void IEventListener::UnregisterAllEventCallbacks()
{
	for (IEventDelegate* cb : this->GetRegisteredCallbacks()) {
		Engine::GetInstance()->UnsubscribeEvent(cb);
	}

	this->GetRegisteredCallbacks().clear();
}

}
}
}