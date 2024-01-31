#pragma once

#include <memory>

#include "ecs/component/component.hpp"

#include "core/events/engine_events.hpp"

#include "SDL2/SDL_video.h"

namespace raptor_engine
{
namespace core
{

class WindowComponent : public ecs::Component<WindowComponent>
{
public:
	WindowComponent() : window {nullptr}
	{ }

	WindowComponent(std::shared_ptr<SDL_Window> window_) : window {window_}
	{ }

	WindowComponent(const WindowComponent&)			   = delete;
	WindowComponent& operator=(const WindowComponent&) = delete;

	~WindowComponent() = default;

public:
	const std::shared_ptr<SDL_Window>& GetWindow() const
	{
		return this->window;
	}

private:
	std::shared_ptr<SDL_Window> window;
};

using WindowComponentUptr = std::unique_ptr<WindowComponent>;
using WindowComponentSptr = std::shared_ptr<WindowComponent>;
using WindowComponentWptr = std::weak_ptr<WindowComponent>;


} // namespace core
} // namespace raptor_enigne