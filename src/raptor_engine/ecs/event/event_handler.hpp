#pragma once

#include <unordered_map>

#include "ecs/event/i_event.hpp"
#include "ecs/event/i_event_delegate.hpp"
#include "ecs/event/event_dispatcher.hpp"

#include "memory/global_memory_user.hpp"
#include "memory/allocators/linear_allocator.hpp"


namespace raptor_engine 
{
class Engine;

namespace ecs 
{
namespace event 
{

class EventHandler : memory::GlobalMemoryUser
{
	friend class Engine;

	using EventDispatcherMap = std::unordered_map<EventTypeId, IEventDispatcher*>;

	using EventStorage		   = std::vector<IEvent*>;
	using EventMemoryAllocator = memory::allocator::LinearAllocator;

private:
	inline void SetEventDispatcherMap(const EventDispatcherMap& eventDispatcherMap)
	{
		this->eventDispatcherMap = eventDispatcherMap;
	}
	inline EventDispatcherMap& GetEventDispatcherMap()
	{
		return this->eventDispatcherMap;
	}
	inline const EventDispatcherMap& GetEventDispatcherMap() const
	{
		return this->eventDispatcherMap;
	}

	inline void SetEventMemoryAllocator(EventMemoryAllocator* eventMemoryAllocator)
	{
		this->eventMemoryAllocator = eventMemoryAllocator;
	}
	inline auto GetEventMemoryAllocator()
	{
		return this->eventMemoryAllocator;
	}
	inline const auto GetEventMemoryAllocator() const
	{
		return this->eventMemoryAllocator;
	}

	inline void SetEventStorage(const EventStorage& eventStorage)
	{
		this->eventStorage = eventStorage;
	}
	inline auto& GetEventStorage()
	{
		return this->eventStorage;
	}
	inline const auto& GetEventStorage() const
	{
		return this->eventStorage;
	}

public:
	EventHandler();
	~EventHandler();

public:

	inline void ClearEventBuffer()
	{
		this->GetEventMemoryAllocator()->Clear();
		this->GetEventStorage().clear();
	}

	inline void ClearEventDispatcher()
	{
		this->GetEventDispatcherMap().clear();
	}

	template<typename E, typename... Args>
	void Send(Args&&... eventArgs)
	{
		// static_assert(std::is_trivially_copyable<E>::value,
		//               "Event is not trivially copyable.");

		void* pMem = this->GetEventMemoryAllocator()->Allocate(sizeof(E), alignof(E));

		if (pMem != nullptr) 
		{
			this->GetEventStorage().push_back(new (pMem) E(std::forward<Args>(eventArgs)...));
			//LogTrace("%s event buffered.", typeid(E).name());
		} 
		else 
		{
			//LogWarning("Event buffer is full! Cut off new incoming events !!!");
		}
	}

	void DispatchEvents();

private:

	EventHandler(const EventHandler&)			 = delete;
	EventHandler& operator=(const EventHandler&) = delete;

private:

	// Add event callback
	template<class E>
	inline void AddEventCallback(const IEventDelegateSptr& eventDelegate)
	{
		EventTypeId ETID = E::STATIC_EVENT_TYPE_ID;

		EventDispatcherMap::const_iterator iter = this->GetEventDispatcherMap().find(ETID);
		if (iter == this->GetEventDispatcherMap().end()) 
		{
			std::pair<EventTypeId, IEventDispatcher*> kvp(ETID, new EventDispatcher<E>());

			kvp.second->AddEventCallback(eventDelegate);

			this->GetEventDispatcherMap().insert(kvp);
		} 
		else 
		{
			this->GetEventDispatcherMap()[ETID]->AddEventCallback(eventDelegate);
		}
	}

	// Remove event callback
	inline void RemoveEventCallback(const IEventDelegateSptr& eventDelegate)
	{
		auto							   typeId = eventDelegate->GetStaticEventTypeId();
		EventDispatcherMap::const_iterator iter	  = this->GetEventDispatcherMap().find(typeId);
		if (iter != this->GetEventDispatcherMap().end()) 
		{
			this->GetEventDispatcherMap()[typeId]->RemoveEventCallback(eventDelegate);
		}
	}

private:
	EventDispatcherMap	  eventDispatcherMap;
	EventMemoryAllocator* eventMemoryAllocator;
	EventStorage		  eventStorage;
};

using EventHandlerUptr = std::unique_ptr<EventHandler>;
using EventHandlerSptr = std::shared_ptr<EventHandler>;
using EventHandlerWptr = std::weak_ptr<EventHandler>;

} // namespace event
} // namespace ecs
} // namespace raptor_engine