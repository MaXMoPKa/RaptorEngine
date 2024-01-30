#include "core/systems/hardware_system.hpp"

#include "SDL2/SDL.h"        // for SDL_Init, SDL_Quit, SDL_INIT_VIDEO
#include "SDL2/SDL_error.h"  // for SDL_GetError
#include "SDL2/SDL_log.h"    // for SDL_LogCritical, SDL_LOG_CATEGORY_ERROR

namespace raptor_engine
{
namespace core
{

HardwareSystem::HardwareSystem()
{
	this->RegisterEventCallbacks();
}

HardwareSystem::~HardwareSystem()
{
	this->UnregisterEventCallbacks();
}

void HardwareSystem::OnReceiveInitializeEvent(const events::ReceiveInitializeEvent* event) 
{
	this->Initialize(event->hardwareSystemInitFlags);
}

void HardwareSystem::Initialize(u32 initFlags_) 
{
	const auto result = SDL_Init(initFlags_);

	if (result != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s", SDL_GetError());
		return;
	}
}


void HardwareSystem::RegisterEventCallbacks()
{
	RegisterEventCallback(&HardwareSystem::OnReceiveInitializeEvent);
}

void HardwareSystem::UnregisterEventCallbacks()
{
	UnregisterEventCallback(&HardwareSystem::OnReceiveInitializeEvent);
}

}
}