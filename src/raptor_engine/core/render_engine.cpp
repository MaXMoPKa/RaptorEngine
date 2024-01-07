#include "core/render_engine.hpp"

#include "render/forward_ldr_renderer.hpp"
#include "render/renderable_object.hpp"

using namespace raptor_engine::render;

class render_engine::render_engine_pimpl
{
public:

	render_engine_pimpl() : renderer {} { }

	render_engine_pimpl(const init_render_engine_data_sptr& init_render_engine_data)
	{ 
		switch (init_render_engine_data->get_renderer_type())
		{
			case high_level_renderer_type::FORWARD_LDR_RENDERER:
			{
				renderer = std::make_shared<forward_ldr_renderer>(init_render_engine_data->get_renderer_info(),
																  init_render_engine_data->get_scene_info());
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

	void create(const init_render_engine_data_sptr& init_render_engine_data)
	{
		render_engine_pimpl tmp {init_render_engine_data};
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

	void update(u64 time_)
	{
		this->renderer->update(renderable_objects, time_);
	}

	void post_update()
	{
		this->renderer->post_update();
	}

public:
	void set_renderable_objects(const std::vector<renderable_object_sptr>& renderable_objects_)
	{
		renderable_objects = std::move(renderable_objects_);
	}


public:
	inline base_renderer_sptr get_renderer() const
	{
	    return this->renderer;
	}

private:
	base_renderer_sptr renderer;

	std::vector<renderable_object_sptr> renderable_objects;
};

render_engine::render_engine() : pimpl {std::make_unique<render_engine_pimpl>()} { }

render_engine::render_engine(const init_render_engine_data_sptr& init_render_engine_data)
	: pimpl {std::make_unique<render_engine_pimpl>(init_render_engine_data)}
{ }

render_engine::render_engine(render_engine&& render_eng) noexcept = default;

render_engine& render_engine::operator=(render_engine&& render_eng) noexcept = default;

render_engine::~render_engine() = default;

void render_engine::create(const init_render_engine_data_sptr& init_render_engine_data)
{
	this->pimpl->create(init_render_engine_data);
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

void render_engine::update(u64 time_)
{
	this->pimpl->update(time_);
}

void render_engine::post_update()
{
	this->pimpl->post_update();
}

void raptor_engine::render::render_engine::set_renderable_objects(
	const std::vector<renderable_object_sptr>& renderable_objects_)
{ 
	this->pimpl->set_renderable_objects(renderable_objects_);
}

base_renderer_sptr render_engine::get_renderer() const
{
    return this->pimpl->get_renderer();
}