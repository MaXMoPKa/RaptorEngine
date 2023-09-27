#pragma once

#include "defines.hpp"

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

	virtual void use_shader_program();

	virtual void bind_vao();

	virtual void draw_arrays();

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