#pragma once

#include "ecs/platform.hpp"

namespace raptor_engine {
namespace ecs {
namespace event {

using EventTypeId    = type_id;
using EventTimeStamp = time_stamp;

static const EventTypeId INVALID_EVENT_TYPE = INVALID_TYPE_ID;

class IEvent
{
public:

	IEvent(EventTypeId id_);
	virtual ~IEvent();

	inline const EventTypeId get_id() const
	{
		return this->id;
	}
	inline const EventTimeStamp get_time_created() const
	{
		return this->timeCreated;
	}

private:
	inline void set_id(const EventTypeId id_)
	{
		this->id = id_;
	}

	inline void set_time_created(const EventTimeStamp timeCreated_)
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