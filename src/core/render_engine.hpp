#pragma once 

#include "render/forward_ldr_renderer.hpp"

namespace raptor_engine {
namespace render {

class render_engine
{
public:
	render_engine() { }

	render_engine(const sdl_window_sptr& window_ptr) : renderer {std::make_shared<forward_ldr_renderer>(window_ptr)} { }

	render_engine(render_engine&& renderer) noexcept			= default;
	render_engine& operator=(render_engine&& renderer) noexcept = default;

	render_engine(const render_engine&)			   = delete;
	render_engine& operator=(const render_engine&) = delete;

	~render_engine() { }

public:

	void pre_update()
	{
		renderer->pre_update();
	}

	void update() 
	{
		renderer->update();
	}

	void post_update()
	{
		renderer->post_update();
	}

public:

	void create(const sdl_window_sptr& window_ptr)
	{
		render_engine tmp {window_ptr};
		this->swap(tmp);
	}

	void swap(render_engine& render_engine) noexcept
	{
		if (this == &render_engine) {
			return;
		}

		// std::swap(this->data, render_engine.data);
	}

	void reset() noexcept
	{
		render_engine tmp {};
		this->swap(tmp);
	}

private:
	// render_engine_data_sptr data;
	base_renderer_sptr renderer;
};

using render_engine_uptr = std::unique_ptr<render_engine>;
using render_engine_sptr = std::shared_ptr<render_engine>;
using render_engine_wptr = std::weak_ptr<render_engine>;

}
}