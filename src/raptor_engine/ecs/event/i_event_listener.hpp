#pragma once

#include <list>

#include "ecs/event/event_delegate.hpp"

#include "core/engine.hpp"

namespace raptor_engine {
namespace ecs {
namespace event {

class IEventListener
{
    using RegisteredCallbacks = std::list<IEventDelegateSptr>;

public:
	IEventListener() = default;
	virtual ~IEventListener();

public:
	template <typename E, typename C>
	inline void RegisterEventCallback(void (C::*Callback)(const E))
	{
		IEventDelegateSptr eventDelegate = std::make_shared<EventDelegate<C, E>>(static_cast<C*>(this), Callback);

		this->GetRegisteredCallbacks().push_back(eventDelegate);
		Engine::GetInstance()->SubscribeEvent<E>(eventDelegate);
	}

	template<typename E, typename C>
	inline void UnregisterEventCallback(void (C::* Callback)(const E))
	{
		EventDelegate<C, E> delegate(static_cast<C*>(this), Callback);

		for (const IEventDelegateSptr& cb : this->registeredCallbacks)
		{
			if (cb->GetId() == delegate.GetId())
			{
				this->GetRegisteredCallbacks().remove_if([&](const IEventDelegateSptr& other_) { return other_ == cb; });

				Engine::GetInstance()->UnsubscribeEvent(&delegate);
				break;
			}
		}
	}

	void UnregisterAllEventCallbacks();

private:

	inline void SetRegisteredCallbacks(const RegisteredCallbacks& registeredCallbacks_)
	{
		this->registeredCallbacks = registeredCallbacks_;
	}

	inline auto& GetRegisteredCallbacks()
	{
		return this->registeredCallbacks;
	}
	inline const auto& GetRegisteredCallbacks() const
	{
		return this->registeredCallbacks;
	}

private:
	RegisteredCallbacks registeredCallbacks;
};

using IEventListenerUptr = std::unique_ptr<IEventListener>;
using IEventListenerSptr = std::shared_ptr<IEventListener>;
using IEventListenerWptr = std::weak_ptr<IEventListener>;

} // namespace event
} // namespace ecs
} // namespace raptor_engine