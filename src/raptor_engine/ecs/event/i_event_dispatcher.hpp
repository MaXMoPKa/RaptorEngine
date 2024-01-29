#pragma once

#include <cstddef>

namespace raptor_engine {
namespace ecs {
namespace event {

class IEvent;
class IEventDelegate;

class IEventDispatcher
{
public:
	IEventDispatcher()			= default;
	virtual ~IEventDispatcher() = default;

public:
	virtual void Dispatch(IEvent* event) = 0;

	virtual void AddEventCallback(const IEventDelegateSptr& eventDelegate_) = 0;

	virtual void RemoveEventCallback(const IEventDelegateSptr& eventDelegate_) = 0;

	virtual std::size_t GetEventCallbackCount() const = 0;

};

} // namespace event
} // namespace ecs
} // namespace raptor_engine