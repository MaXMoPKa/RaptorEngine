#pragma once

#include "render/ogl_render_api.hpp"

namespace raptor_engine {
namespace render { 

class base_renderer
{
public:
	base_renderer() { }

	base_renderer(const sdl_window_sptr& window_ptr)
		: render_api {std::make_shared<ogl_render_api>(window_ptr)}
	{ }

	base_renderer(base_renderer&& renderer) noexcept            = default;
	base_renderer& operator=(base_renderer&& renderer) noexcept = default;

	base_renderer(const base_renderer&)			   = delete;
	base_renderer& operator=(const base_renderer&) = delete;

	virtual ~base_renderer() { }

public:

	virtual void pre_update() { }

	virtual void update() { }

	virtual void post_update() { }

public:

	virtual void create(const sdl_window_sptr& window_ptr)
	{
		base_renderer tmp {window_ptr};
		this->swap(tmp);
	}

	virtual void swap(base_renderer& renderer) noexcept
	{
		if (this == &renderer) {
			return;
		}

		std::swap(this->render_api, renderer.render_api);
	}

	virtual void reset() noexcept
	{
		base_renderer tmp {};
		this->swap(tmp);
	}

protected:
	base_render_api_sptr render_api;
};

using base_renderer_uptr = std::unique_ptr<base_renderer>;
using base_renderer_sptr = std::shared_ptr<base_renderer>;
using base_renderer_wptr = std::weak_ptr<base_renderer>;

}
}