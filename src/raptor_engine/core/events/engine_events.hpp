#pragma once

#include "defines.hpp"

#include "ecs/event/event.hpp"

namespace raptor_engine
{
namespace core
{
namespace events
{
    
struct SendInitializeEvent : public ecs::event::Event<SendInitializeEvent> { };
struct ReceiveInitializeEvent : public ecs::event::Event<ReceiveInitializeEvent> 
{
	u32 hardwareSystemInitFlags;

	ReceiveInitializeEvent(u32 hardwareSystemInitFlags_) : hardwareSystemInitFlags {hardwareSystemInitFlags_}
	{ }
};

}
}
}