#include "render/base_render_api.hpp"

using namespace raptor_engine::render;

class base_render_api::base_render_api_pimpl
{
public:
	base_render_api_pimpl() : window {} { }

	base_render_api_pimpl(const sdl_window_sptr& window_ptr)
		: window {window_ptr} { }

	~base_render_api_pimpl() = default;

public:

	void create(const sdl_window_sptr& window_ptr)
	{
		base_render_api_pimpl tmp {window_ptr};
		this->swap(tmp);
	}

	void swap(base_render_api_pimpl& pimpl) noexcept
	{
		if (this == &pimpl) {
			return;
		}

		std::swap(this->window, pimpl.window);
	}

	void reset() noexcept
	{
		base_render_api_pimpl tmp {};
		this->swap(tmp);
	}

public:
	void clear_color() { }

	void use_shader_program(u32 shader_id) { }

	void bind_g_uniforms(const std::vector<u32>& g_uniforms_, u64 time_) { }

	void bind_vao(u32 vao_id) { }

	void draw_arrays() { }

	void draw_elements(draw_config_sptr draw_config) { }

	void swap_window() { }

public:
	inline sdl_window_wptr get_window() const
	{
		return this->window;
	}

private:
	sdl_window_wptr window;
};

base_render_api::base_render_api() : pimpl{std::make_unique<base_render_api_pimpl>()} { }

base_render_api::base_render_api(const sdl_window_sptr& window_ptr)
	: pimpl {std::make_unique<base_render_api_pimpl>(window_ptr)}
{ }

base_render_api::base_render_api(base_render_api&& api) noexcept = default;

base_render_api& base_render_api::operator=(base_render_api&& api) noexcept = default;

base_render_api::~base_render_api() = default;

void base_render_api::create(const sdl_window_sptr& window_ptr)
{
	this->pimpl->create(window_ptr);
}

void base_render_api::swap(base_render_api& api) noexcept
{
	this->pimpl.swap(api.pimpl);
}

void base_render_api::reset() noexcept
{
	this->pimpl->reset();
}

void base_render_api::clear_color()
{
	this->pimpl->clear_color();
}

void base_render_api::use_shader_program(u32 shader_id)
{
	this->pimpl->use_shader_program(shader_id);
}

void base_render_api::bind_g_uniforms(const std::vector<u32>& g_uniforms_, u64 time_) 
{
	this->pimpl->bind_g_uniforms(g_uniforms_, time_);
}

void base_render_api::bind_vao(u32 vao_id)
{
	this->pimpl->bind_vao(vao_id);
}

void base_render_api::draw_arrays()
{
	this->pimpl->draw_arrays();
}

void base_render_api::draw_elements(draw_config_sptr draw_config)
{
	this->pimpl->draw_elements(draw_config);
}

void base_render_api::swap_window()
{
	this->pimpl->swap_window();
}

sdl_window_wptr base_render_api::get_window() const 
{ 
	return this->pimpl->get_window();
}