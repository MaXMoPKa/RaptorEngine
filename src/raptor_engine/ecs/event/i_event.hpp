#pragma once

#include "ecs/utils/platform.hpp"

namespace raptor_engine {
namespace ecs {
namespace event {

using EventTypeId    = TypeId;
using EventTimeStamp = TimeStamp;

static const EventTypeId INVALID_EVENT_TYPE = INVALID_TYPE_ID;

class IEvent
{
public:

	IEvent(EventTypeId id_);
	virtual ~IEvent();

	inline const EventTypeId GetId() const
	{
		return this->id;
	}
	inline const EventTimeStamp GetTimeCreated() const
	{
		return this->timeCreated;
	}

private:
	inline void SetId(const EventTypeId id_)
	{
		this->id = id_;
	}

	inline void SetTimeCreated(const EventTimeStamp timeCreated_)
	{
		this->timeCreated = timeCreated_;
	}

private:
	EventTypeId	   id;
	EventTimeStamp timeCreated;
};

} // namespace event
} // namespace ecs
} // namespace raptor_engine