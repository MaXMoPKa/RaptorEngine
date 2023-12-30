#include "render/base_renderer.hpp"

using namespace raptor_engine::render;

class base_renderer::base_renderer_pimpl
{
public:
	base_renderer_pimpl() : render_api {} { }

	base_renderer_pimpl(const high_level_renderer_data_sptr& renderer_data, const scene_data_sptr& scene_info)
	{ 
		switch (renderer_data->low_level_render_api) {
			case low_level_render_api_type::OPENGL: {
				render_api = std::make_shared<ogl_render_api>(renderer_data->window_ptr, scene_info);
				break;
			}
			case low_level_render_api_type::DIRECTX:
			case low_level_render_api_type::METALL:
			case low_level_render_api_type::VULKAN:
			default: {
				break;
			}
		}
	}

	~base_renderer_pimpl() { }

public:

	void create(const high_level_renderer_data_sptr& renderer_data, const scene_data_sptr& scene_info)
	{
		base_renderer_pimpl tmp {renderer_data, scene_info};
		this->swap(tmp);
	}

	void swap(base_renderer_pimpl& pimpl) noexcept
	{
		if (this == &pimpl) {
			return;
		}

		std::swap(this->render_api, pimpl.render_api);
	}

	void reset() noexcept
	{
		base_renderer_pimpl tmp {};
		this->swap(tmp);
	}

public:
	void pre_update() { }

	void update(const std::vector<renderable_object_sptr>& renderable_objects_, u64 time_) { }

	void post_update() { }

public:
	inline base_render_api_sptr get_render_api() const
	{
		return this->render_api;
	}
	
private:
	base_render_api_sptr render_api;
};

base_renderer::base_renderer() : pimpl{std::make_unique<base_renderer_pimpl>()} { }

base_renderer::base_renderer(const high_level_renderer_data_sptr& renderer_data, const scene_data_sptr& scene_info)
	: pimpl {std::make_unique<base_renderer_pimpl>(renderer_data, scene_info)}
{ }

base_renderer::base_renderer(base_renderer&& renderer) noexcept = default;

base_renderer& base_renderer::operator=(base_renderer&& renderer) noexcept = default;

base_renderer::~base_renderer() = default;

void base_renderer::create(const high_level_renderer_data_sptr& renderer_data, const scene_data_sptr& scene_info)
{
	this->pimpl->create(renderer_data, scene_info);
}

void base_renderer::swap(base_renderer& renderer) noexcept
{
	this->pimpl.swap(renderer.pimpl);
}

void base_renderer::reset() noexcept
{
	this->pimpl->reset();
}

void base_renderer::pre_update()
{
	this->pimpl->pre_update();
}

void base_renderer::update(const std::vector<renderable_object_sptr>& renderable_objects_, u64 time_)
{
	this->pimpl->update(renderable_objects_, time_);
}

void base_renderer::post_update()
{
	this->pimpl->post_update();
}

base_render_api_sptr base_renderer::get_render_api() const
{
	return this->pimpl->get_render_api();
}