#pragma once

#include <typeinfo>

#include "defines.hpp"

namespace raptor_engine {
namespace ecs {
namespace event {

class IEvent;

using EventDelegateId = std::size_t;

class IEventDelegate
{
public:
	IEventDelegate()			= default;
	virtual ~IEventDelegate() = default;

public:
	virtual inline void Invoke(const IEvent* const e) = 0;
	
	virtual inline EventDelegateId GetId() const	= 0;
	virtual inline u64 GetStaticEventTypeId() const = 0;
	
	virtual bool operator==(const IEventDelegate* other) const = 0;

	virtual IEventDelegate* Clone() = 0;
};


} // namespace event
} // namespace ecs
} // namespace raptor_engine