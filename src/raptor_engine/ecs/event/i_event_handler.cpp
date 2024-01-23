#include "ecs/event/i_event_handler.hpp"

using namespace raptor_engine::ecs::event;

EventHandler::EventHandler()
{
	//DEFINE_LOGGER("EventHandler")
	//LogInfo("Initialize EventHandler!");

	this->eventMemoryAllocator =
		new EventMemoryAllocator(ECS_EVENT_MEMORY_BUFFER_SIZE, allocate(ECS_EVENT_MEMORY_BUFFER_SIZE, "EventHandler"));

	this->GetEventStorage().reserve(1024);
}

EventHandler::~EventHandler()
{
	for (EventHandler::EventDispatcherMap::iterator it = this->GetEventDispatcherMap().begin();
		 it != this->GetEventDispatcherMap().end();
		 ++it) 
	{
		delete (*it).second;
		(*it).second = nullptr;
	}

	this->GetEventDispatcherMap().clear();

	// Release allocated memory
	this->free((void*)this->GetEventMemoryAllocator()->get_address());

	delete this->GetEventMemoryAllocator();
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