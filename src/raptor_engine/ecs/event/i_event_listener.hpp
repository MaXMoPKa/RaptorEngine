#pragma once

#include <list>

#include "core/engine.hpp"

#include "ecs/event/event_delegate.hpp"

namespace raptor_engine
{
class Engine;
}

namespace raptor_engine {
namespace ecs {
namespace event {

class IEventListener
{
    using RegisteredCallbacks = std::list<IEventDelegate*>;

public:
	IEventListener() = default;
	virtual ~IEventListener();

public:
	template <typename E, typename C>
	inline void RegisterEventCallback(void (C::*Callback)(const E* const))
	{
		IEventDelegate* eventDelegate = new EventDelegate<C, E>(static_cast<C*>(this), Callback);

		this->GetRegisteredCallbacks().push_back(eventDelegate);
		Engine::GetInstance()->SubscribeEvent<E>(eventDelegate);
	}

	template<typename E, typename C>
	inline void UnregisterEventCallback(void (C::* Callback)(const E* const))
	{
		EventDelegate<C, E> delegate(static_cast<C*>(this), Callback);

		for (const IEventDelegate* cb : this->registeredCallbacks)
		{
			if (cb->GetId() == delegate.GetId())
			{
				this->GetRegisteredCallbacks().remove_if([&](const IEventDelegate* other_) { return other_ == cb; });

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