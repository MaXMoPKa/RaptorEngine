#pragma once

#include <list>

#include "ecs/event/i_event_dispatcher.hpp"

namespace raptor_engine {
namespace ecs {
namespace event {

template<typename T>
class EventDispatcher : public IEventDispatcher
{
	using EventDelegateList		 = std::list<IEventDelegateSptr>;
	using PendingRemoveDelegates = std::list<IEventDelegateSptr>;

public:
	EventDispatcher() : locked(false) { }
	virtual ~EventDispatcher()
	{
		this->GetPendingRemoveDelegates().clear();
		this->GetEventCallbacks().clear();
	}

	virtual void Dispatch(const IEventSptr& event) override
	{
		this->SetLocked(true);
		LogTrace("Dispatch event %s", typeid(T).name());
		if (this->GetPendingRemoveDelegates().empty() == false) 
		{
			for (auto EC : this->GetPendingRemoveDelegates()) 
			{
				auto result = std::find_if(this->GetEventCallbacks().begin(),
										   this->GetEventCallbacks().end(),
										   [&](const IEventDelegateSptr& other) { return other->operator==(EC); });
				if (result != this->GetEventCallbacks().end()) 
				{
					IEventDelegateSptr ptrMem = static_cast<IEventDelegateSptr>(*result);
					this->GetEventCallbacks().erase(result);
					ptrMem.reset();
					ptrMem = nullptr;
				}
			}
			this->GetPendingRemoveDelegates().clear();
		}

		for (auto EC : this->GetEventCallbacks()) 
		{
			assert(EC != nullptr && "Invalid event callback.");
			EC->invoke(event);
		}

		this->SetLocked(false);
	}
	virtual void AddEventCallback(const IEventDelegateSptr& eventDelegate) override
	{
		auto result = std::find_if(this->GetPendingRemoveDelegates().begin(),
								   this->GetPendingRemoveDelegates().end(),
								   [&](const IEventDelegateSptr& other) { return other->operator==(eventDelegate); });

		if (result != this->GetPendingRemoveDelegates().end()) 
		{
			this->GetPendingRemoveDelegates().erase(result);
			return;
		}

		this->GetEventCallbacks().push_back(eventDelegate);
	}
	virtual void RemoveEventCallback(const IEventDelegateSptr& eventDelegate) override
	{
		if (this->GetLocked() == false) 
		{
			auto result = std::find_if(this->GetEventCallbacks().begin(),
									   this->GetEventCallbacks().end(),
									   [&](const IEventDelegateSptr& other) { return other->operator==(eventDelegate); });

			if (result != this->GetEventCallbacks().end()) 
			{
				IEventDelegateSptr ptrMem = static_cast<IEventDelegateSptr>(*result);

				this->GetEventCallbacks().erase(result);

				ptrMem.reset();
				ptrMem = nullptr;
			}
		} 
		else 
		{
			auto result = std::find_if(this->GetEventCallbacks().begin(),
									   this->GetEventCallbacks().end(),
									   [&](const IEventDelegateSptr& other) { return other->operator==(eventDelegate); });
			// assert(result != this->GetEventCallbacks().end() && "");
			if (result != this->GetEventCallbacks().end()) 
			{
				this->GetPendingRemoveDelegates().push_back((*result));
			}
		}
	}
	virtual std::size_t GetEventCallbackCount() const override
	{
		return this->GetEventCallbacks().size();
	}

private:
	inline void SetPendingRemoveDelegates(const PendingRemoveDelegates& pendingRemoveDelegates)
	{
		this->pendingRemoveDelegates = pendingRemoveDelegates;
	}
	inline auto& GetPendingRemoveDelegates()
	{
		return this->pendingRemoveDelegates;
	}
	inline const auto& GetPendingRemoveDelegates() const
	{
		return this->pendingRemoveDelegates;
	}

	inline void SetEventCallbacks(const EventDelegateList& eventCallbacks)
	{
		this->eventCallbacks = eventCallbacks;
	}
	inline auto& GetEventCallbacks()
	{
		return this->eventCallbacks;
	}
	inline const auto& GetEventCallbacks() const
	{
		return this->eventCallbacks;
	}

	inline void SetLocked(bool locked)
	{
		this->locked = locked;
	}
	inline auto& GetLocked()
	{
		return this->locked;
	}
	inline const auto& GetLocked() const
	{
		return this->locked;
	}

private:
	PendingRemoveDelegates pendingRemoveDelegates;
	EventDelegateList	   eventCallbacks;
	bool				   locked;
};

} // namespace event
} // namespace ecs
} // namespace raptor_engine