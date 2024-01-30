#include "ecs/event/event_handler.hpp"

using namespace raptor_engine::ecs::event;

EventHandler::EventHandler()
{
	//DEFINE_LOGGER("EventHandler")
	//LogInfo("Initialize EventHandler!");

	this->eventMemoryAllocator =
		std::make_shared<EventMemoryAllocator>(ECS_EVENT_MEMORY_BUFFER_SIZE, Allocate(ECS_EVENT_MEMORY_BUFFER_SIZE, "EventHandler"));

	this->GetEventStorage().reserve(1024);
}

EventHandler::~EventHandler()
{
	for (EventHandler::EventDispatcherMap::iterator it = this->GetEventDispatcherMap().begin();
		 it != this->GetEventDispatcherMap().end();
		 ++it) 
	{
		(*it).second.reset();
		(*it).second = nullptr;
	}

	this->GetEventDispatcherMap().clear();

	// Release allocated memory
	this->Free((void*)this->GetEventMemoryAllocator()->GetAddress());

	this->GetEventMemoryAllocator().reset();
	this->SetEventMemoryAllocator(nullptr);

	//LogInfo("Relealse EventHandler!");
}

void EventHandler::DispatchEvents()
{
	std::size_t lastIndex = this->GetEventStorage().size();
	std::size_t thisIndex = 0;

	while (thisIndex < lastIndex) 
	{
		auto event = this->GetEventStorage()[thisIndex++];
		if (event == nullptr) 
		{
			//LogError("Skip corrupted event.", event->GetTypeID());
			continue;
		}

		auto it = this->GetEventDispatcherMap().find(event->GetId());
		if (it == this->GetEventDispatcherMap().end()) 
		{
			continue;
		}

		it->second->Dispatch(event);

		lastIndex = this->GetEventStorage().size();
	}

	this->ClearEventBuffer();
}