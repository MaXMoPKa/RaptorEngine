#pragma once

#include <memory>

#include "SDL_log.h"

#include "glad/glad.h"

#include "render/base_render_api.hpp"

#include "defines.hpp"

namespace raptor_engine {
namespace render {

class ogl_render_api : public base_render_api
{
public:
	ogl_render_api() : window {}, gl_context {nullptr} { }

	ogl_render_api(sdl_window_sptr& window_ptr) : window {window_ptr}, gl_context {nullptr} 
	{ 
		init();
	}

	ogl_render_api(ogl_render_api&& api) noexcept            = default;
	ogl_render_api& operator=(ogl_render_api&& api) noexcept = default;

	ogl_render_api(const ogl_render_api&)			 = delete;
	ogl_render_api& operator=(const ogl_render_api&) = delete;
 
	virtual ~ogl_render_api() { }

public:
	void create(sdl_window_sptr& window_ptr) 
	{
		ogl_render_api tmp(window_ptr);
		this->swap(tmp);

	}

	void swap(ogl_render_api& api) noexcept
	{
		if (this == &api) 
		{
			return;
		}

		std::swap(this->window, api.window);
		std::swap(this->gl_context, api.gl_context);
	}

	void reset() noexcept
	{
		ogl_render_api tmp {};
		this->swap(tmp);
	}

public:
	void swap_window() 
	{ 
		SDL_GL_SwapWindow(this->window.lock().get());
	}

private:
	void init()
	{
		if (int result = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) != 0) {
			SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Failed to initialize GLAD with error code: %n", result);
		}

		gl_context = SDL_GL_CreateContext(this->window.lock().get());
	}

private:
	sdl_window_wptr window;
	SDL_GLContext gl_context;
};

using ogl_render_api_uptr = std::unique_ptr<ogl_render_api>;
using ogl_render_api_sptr = std::shared_ptr<ogl_render_api>;
using ogl_render_api_wptr = std::weak_ptr<ogl_render_api>;

} // namespace render
} // namespace raptor_engine