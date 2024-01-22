#pragma once

#include "ecs/event/i_event.hpp"

namespace raptor_engine {
namespace ecs {
namespace event {

template <typename T>
class Event : public IEvent
{
public:
	static const EventTypeId STATIC_EVENT_TYPE_ID;
	
	Event() 
		: IEvent {STATIC_EVENT_TYPE_ID}
	{
	}
};

template <typename T>
const EventTypeId Event<T>::STATIC_EVENT_TYPE_ID {typeid(T).hash_code()};

} // namespace event
} // namespace ecs
} // namespace raptor_engine