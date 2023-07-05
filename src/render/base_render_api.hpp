#pragma once

#include "defines.hpp"

namespace raptor_engine {
namespace render {

class base_render_api
{
public:
	base_render_api() : window {} { }

	base_render_api(const sdl_window_sptr& window_ptr) : window {window_ptr} { }

	base_render_api(base_render_api&& api) noexcept			   = default;
	base_render_api& operator=(base_render_api&& api) noexcept = default;

	base_render_api(const base_render_api&)			  = delete;
	base_render_api& operator=(const base_render_api&) = delete;

	virtual ~base_render_api() { }

public:

	virtual void create(const sdl_window_sptr& window_ptr) 
	{
		base_render_api tmp {};
		this->swap(tmp);
	}

	virtual void swap(base_render_api& api) noexcept
	{
		if (this == &api) 
		{
			return;
		}

		std::swap(this->window, api.window);
	}

	virtual void reset() noexcept 
	{
		base_render_api tmp {};
		this->swap(tmp);
	}

public:
	virtual void clear_color() { }

	virtual void use_shader_program() { }

	virtual void bind_vao() { }

	virtual void draw_arrays() { }

	virtual void swap_window() { }

protected:

	sdl_window_wptr window;

};

using base_render_api_uptr = std::unique_ptr<base_render_api>;
using base_render_api_sptr = std::shared_ptr<base_render_api>;
using base_render_api_wptr = std::weak_ptr<base_render_api>;

} // namespace render_engine
} // namespace raptor_engine