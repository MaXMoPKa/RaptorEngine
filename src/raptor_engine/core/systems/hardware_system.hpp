#pragma once

#include <memory>

#include "ecs/system/system.hpp"
#include "ecs/event/i_event_listener.hpp"

#include "core/events/engine_events.hpp"

namespace raptor_engine
{
namespace core
{

class HardwareSystem : public ecs::System<HardwareSystem>,
					   public ecs::event::IEventListener
{
public:
	HardwareSystem();

	HardwareSystem(const HardwareSystem&)            = delete;
	HardwareSystem& operator=(const HardwareSystem&) = delete;

	~HardwareSystem();

public:
	void OnReceiveInitializeEvent(const events::ReceiveInitializeEvent* event);

private:
	void Initialize(u32 initFlags_);

private:
	void RegisterEventCallbacks();
	void UnregisterEventCallbacks();

};

using HardwareSystemUptr = std::unique_ptr<HardwareSystem>;
using HardwareSystemSptr = std::shared_ptr<HardwareSystem>;
using HardwareSystemWptr = std::weak_ptr<HardwareSystem>;


} // namespace core
} // namespace raptor_enigne