#include "core/systems/window_system.hpp"

#include "core/components/window_component.hpp"

#include "SDL2/SDL.h"        // for SDL_Init, SDL_Quit, SDL_INIT_VIDEO
#include "SDL2/SDL_error.h"  // for SDL_GetError
#include "SDL2/SDL_log.h"    // for SDL_LogCritical, SDL_LOG_CATEGORY_ERROR

namespace raptor_engine
{
namespace core
{

WindowSystem::WindowSystem() 
	: window {nullptr}
{
	this->RegisterEventCallbacks();
}

WindowSystem::~WindowSystem()
{
	this->UnregisterEventCallbacks();
}

void WindowSystem::OnReceiveCreateWindowEvent(const events::ReceiveInitializeEvent* event)
{
	this->CreateWindow();
}

void WindowSystem::OnWindowRequestEvent(const events::WindowRequestEvent* event_) 
{
	Engine::GetInstance()->SendEvent<events::WindowSendEvent>(this->window);
}

void WindowSystem::CreateWindow()
{
	this->window =
		std::shared_ptr<SDL_Window>(SDL_CreateWindow("Default Title",
													 SDL_WINDOWPOS_CENTERED,
													 SDL_WINDOWPOS_CENTERED,
													 1080,
													 720,
			                                         SDL_WINDOW_OPENGL),
									                 SDL_DestroyWindow);

	if (this->window.get() == nullptr) {
		SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
		return;
	}
}


void WindowSystem::RegisterEventCallbacks()
{
	RegisterEventCallback(&WindowSystem::OnReceiveCreateWindowEvent);
}

void WindowSystem::UnregisterEventCallbacks()
{
	UnregisterEventCallback(&WindowSystem::OnReceiveCreateWindowEvent);
}

}
}