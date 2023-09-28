#include "core/render_engine.hpp"

#include "render/forward_ldr_renderer.hpp"

using namespace raptor_engine::render;

class render_engine::render_engine_pimpl
{
public:

	render_engine_pimpl() : renderer {} { }

	render_engine_pimpl(const render_engine_data_sptr& render_engine_data)
	{ 
		switch (render_engine_data->high_level_renderer)
		{
			case high_level_renderer_type::FORWARD_LDR_RENDERER:
			{
				renderer = std::make_shared<forward_ldr_renderer>(render_engine_data->renderer_data, render_engine_data->scene_data);
				break;
			}
			case high_level_renderer_type::FORWARD_HDR_RENDERER:
			case high_level_renderer_type::DEFFERED_LDR_RENDERER:
			case high_level_renderer_type::DEFERRED_HDR_RENDERER:
			default: 
			{
				break;
			}
		}
	}

	~render_engine_pimpl() { }

public:

	void create(const render_engine_data_sptr& render_engine_data)
	{
		render_engine_pimpl tmp {render_engine_data};
		this->swap(tmp);
	}

	void swap(render_engine_pimpl& pimpl) noexcept
	{
		if (this == &pimpl)
		{
			return;
		}

		std::swap(this->renderer, pimpl.renderer);
	}

	void reset() noexcept
	{
		render_engine_pimpl tmp {};
		this->swap(tmp);
	}


public:
	void pre_update() 
	{ 
		this->renderer->pre_update();
	}

	void update()
	{
		this->renderer->update();
	}

	void post_update()
	{
		this->renderer->post_update();
	}

public:
	inline base_renderer_sptr get_renderer() const
	{
	    return this->renderer;
	}

private:
	base_renderer_sptr renderer;
};

render_engine::render_engine() : pimpl {std::make_unique<render_engine_pimpl>()} { }

render_engine::render_engine(const render_engine_data_sptr& render_engine_data)
	: pimpl {std::make_unique<render_engine_pimpl>(render_engine_data)}
{ }

render_engine::render_engine(render_engine&& render_eng) noexcept = default;

render_engine& render_engine::operator=(render_engine&& render_eng) noexcept = default;

render_engine::~render_engine() = default;

void render_engine::create(const render_engine_data_sptr& render_engine_data)
{
	this->pimpl->create(render_engine_data);
}

void render_engine::swap(render_engine& render_eng) noexcept
{
	this->pimpl.swap(render_eng.pimpl);
}

void render_engine::reset() noexcept
{
	this->pimpl->reset();
}

void render_engine::pre_update()
{
	this->pimpl->pre_update();
}

void render_engine::update()
{
	this->pimpl->update();
}

void render_engine::post_update()
{
	this->pimpl->post_update();
}

base_renderer_sptr render_engine::get_renderer() const
{
    return this->pimpl->get_renderer();
}