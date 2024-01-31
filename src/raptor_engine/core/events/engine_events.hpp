#pragma once

#include "defines.hpp"

#include "ecs/event/event.hpp"

namespace raptor_engine
{
namespace core
{
namespace events
{
    
struct SendInitializeEvent : public ecs::event::Event<SendInitializeEvent> 
{ };
struct ReceiveInitializeEvent : public ecs::event::Event<ReceiveInitializeEvent> 
{
	u32 hardwareSystemInitFlags;

	ReceiveInitializeEvent(u32 hardwareSystemInitFlags_) : hardwareSystemInitFlags {hardwareSystemInitFlags_}
	{ }
};

struct WindowRequestEvent : public ecs::event::Event<WindowRequestEvent> 
{ };
struct WindowSendEvent : public ecs::event::Event<WindowSendEvent>
{ 
    std::shared_ptr<SDL_Window> window;

	WindowSendEvent(std::shared_ptr<SDL_Window> window_) : window {window_}
	{ }
};

}
}
}