#include "render/forward_ldr_renderer.hpp"

using namespace raptor_engine::render;

forward_ldr_renderer::forward_ldr_renderer() : base_renderer() { }

forward_ldr_renderer::forward_ldr_renderer(const high_level_renderer_data_sptr& renderer_data)
	: base_renderer(renderer_data) { }

forward_ldr_renderer::forward_ldr_renderer(forward_ldr_renderer&& renderer) noexcept = default;
forward_ldr_renderer& forward_ldr_renderer::operator=(forward_ldr_renderer&& renderer) noexcept = default;

forward_ldr_renderer::~forward_ldr_renderer() = default;

void forward_ldr_renderer::create(const high_level_renderer_data_sptr& renderer_data)
{
	forward_ldr_renderer tmp {renderer_data};
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

void forward_ldr_renderer::update()
{
	this->get_render_api()->use_shader_program();
	this->get_render_api()->bind_vao();
	this->get_render_api()->draw_arrays();
}

void forward_ldr_renderer::post_update()
{
	this->get_render_api()->swap_window();
}