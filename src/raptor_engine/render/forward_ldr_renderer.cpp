#include "render/forward_ldr_renderer.hpp"

#include "render/renderable_object.hpp"

using namespace raptor_engine::render;

forward_ldr_renderer::forward_ldr_renderer() : base_renderer() { }

forward_ldr_renderer::forward_ldr_renderer(const high_level_renderer_data_sptr& renderer_data,
										   const scene_data_sptr&					scene_info)
	: base_renderer(renderer_data, scene_info) { }

forward_ldr_renderer::forward_ldr_renderer(forward_ldr_renderer&& renderer) noexcept = default;
forward_ldr_renderer& forward_ldr_renderer::operator=(forward_ldr_renderer&& renderer) noexcept = default;

forward_ldr_renderer::~forward_ldr_renderer() = default;

void forward_ldr_renderer::create(const high_level_renderer_data_sptr& renderer_data, const scene_data_sptr& scene_info)
{
	forward_ldr_renderer tmp {renderer_data, scene_info};
	this->swap(tmp);
}

void forward_ldr_renderer::swap(base_renderer& renderer) noexcept
{
	if (this == &renderer) {
		return;
	}

	base_renderer::swap(renderer);
}

void forward_ldr_renderer::reset() noexcept
{
	forward_ldr_renderer tmp {};
	this->swap(tmp);
}

void forward_ldr_renderer::pre_update()
{
	this->get_render_api()->clear_color();
}

void forward_ldr_renderer::update(const std::vector<renderable_object_sptr>& renderable_objects_, u64 time_)
{
	for (const auto& renderable_obj : renderable_objects_)
	{
		this->get_render_api()->use_shader_program(renderable_obj->get_shader_program()->get_id());
		this->get_render_api()->bind_g_uniforms(renderable_obj->get_shader_program()->get_g_uniforms(), time_);
		this->get_render_api()->bind_vao(renderable_obj->get_geometry_object()->get_vao()->get_id());
		this->get_render_api()->bind_texture(renderable_obj->get_texture()->get_id());
		this->get_render_api()->draw_elements(renderable_obj->get_draw_config());
	}
}

void forward_ldr_renderer::post_update()
{
	this->get_render_api()->swap_window();
}