#pragma once

#include <vector>

#include "defines.hpp"

#include "render/draw_config.hpp"

namespace raptor_engine {
namespace render {

class base_render_api
{
public:
	base_render_api();

	base_render_api(const sdl_window_sptr& window_ptr);

	base_render_api(base_render_api&& api) noexcept;
	base_render_api& operator=(base_render_api&& api) noexcept;

	base_render_api(const base_render_api&)			   = delete;
	base_render_api& operator=(const base_render_api&) = delete;

	virtual ~base_render_api();

public:
	virtual void create(const sdl_window_sptr& window_ptr);

	virtual void swap(base_render_api& api) noexcept;

	virtual void reset() noexcept;

public:
	virtual void clear_color();

	virtual void use_shader_program(u32 shader_id);

	virtual void bind_g_uniforms(const std::vector<u32>& g_uniforms_, u64 time_);

	virtual void bind_vao(u32 vao_id);

	virtual void bind_texture(u32 texture_id_, u32 active_texture_id_);

	virtual void draw_arrays();

	virtual void draw_elements(draw_config_sptr draw_config);

	virtual void swap_window();

public:
	[[nodiscard]] sdl_window_wptr get_window() const;

protected:
	class base_render_api_pimpl;
	std::unique_ptr<base_render_api_pimpl> pimpl;
};

using base_render_api_uptr = std::unique_ptr<base_render_api>;
using base_render_api_sptr = std::shared_ptr<base_render_api>;
using base_render_api_wptr = std::weak_ptr<base_render_api>;

} // namespace render_engine
} // namespace raptor_engine