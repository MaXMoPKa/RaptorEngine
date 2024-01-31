#pragma once

#include <memory>

#include "ecs/system/system.hpp"
#include "ecs/event/i_event_listener.hpp"

#include "core/events/engine_events.hpp"

namespace raptor_engine
{
namespace core
{

class WindowSystem : public ecs::System<WindowSystem>,
					 public ecs::event::IEventListener
{
public:
	WindowSystem();

	WindowSystem(const WindowSystem&)			 = delete;
	WindowSystem& operator=(const WindowSystem&) = delete;

	~WindowSystem();

public:
	void OnReceiveCreateWindowEvent(const events::ReceiveInitializeEvent* event_);
	void OnWindowRequestEvent(const events::WindowRequestEvent* event_);

private:
	void CreateWindow();

private:
	void RegisterEventCallbacks();
	void UnregisterEventCallbacks();

private:
	std::shared_ptr<SDL_Window> window;

};

using WindowSystemUptr = std::unique_ptr<WindowSystem>;
using WindowSystemSptr = std::shared_ptr<WindowSystem>;
using WindowSystemWptr = std::weak_ptr<WindowSystem>;


} // namespace core
} // namespace raptor_enigne