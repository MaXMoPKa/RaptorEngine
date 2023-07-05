#pragma once

#include "render/base_renderer.hpp"

namespace raptor_engine {
namespace render {

class forward_ldr_renderer : public base_renderer
{
public:
	forward_ldr_renderer() { }

	forward_ldr_renderer(const sdl_window_sptr& window_ptr) : base_renderer(window_ptr) { }

	forward_ldr_renderer(forward_ldr_renderer&& renderer) noexcept	   = default;
	forward_ldr_renderer& operator=(forward_ldr_renderer&& renderer) noexcept = default;

	forward_ldr_renderer(const forward_ldr_renderer&) = delete;
	forward_ldr_renderer& operator=(const forward_ldr_renderer&) = delete;

	virtual ~forward_ldr_renderer() { }

public:

	void pre_update() override
	{
		render_api->clear_color();
	}

	void update() override
	{
		render_api->use_shader_program();
		render_api->bind_vao();
		render_api->draw_arrays();
	}

	void post_update() override
	{
		render_api->swap_window();
	}

public:

	void create(const sdl_window_sptr& window_ptr) override
	{
		forward_ldr_renderer tmp {window_ptr};
		this->swap(tmp);
	}

	void swap(base_renderer& renderer) noexcept override
	{
		if (this == &renderer) {
			return;
		}

		base_renderer::swap(renderer);
	}

	void reset() noexcept override
	{
		forward_ldr_renderer tmp {};
		this->swap(tmp);
	}
};

using forward_ldr_renderer_uptr = std::unique_ptr<forward_ldr_renderer>;
using forward_ldr_renderer_sptr = std::shared_ptr<forward_ldr_renderer>;
using forward_ldr_renderer_wptr = std::weak_ptr<forward_ldr_renderer>;

}
}
