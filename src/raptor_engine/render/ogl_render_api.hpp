#pragma once

#include "glad/glad.h"

#include <memory>

#include "SDL2/SDL_log.h"

#include "render/base_render_api.hpp"

#include "structs/scene_data.hpp"

using namespace raptor_engine::structs;

namespace raptor_engine {
namespace render {

class ogl_render_api : public base_render_api
{
public:
	ogl_render_api();

	ogl_render_api(const sdl_window_sptr& window_ptr, const scene_data_sptr& scene_info);

	ogl_render_api(ogl_render_api&& api) noexcept;
	ogl_render_api& operator=(ogl_render_api&& api) noexcept;

	ogl_render_api(const ogl_render_api&)			 = delete;
	ogl_render_api& operator=(const ogl_render_api&) = delete;
 
	~ogl_render_api() override;

public:
	void create(const sdl_window_sptr& window_ptr, const scene_data_sptr& scene_info);

	void swap(ogl_render_api& api) noexcept;

	void reset() noexcept override;

public:
	void clear_color() override;

	void use_shader_program() override;

	void bind_vao() override;

	void draw_arrays() override;

	void swap_window() override;

private:
	class ogl_render_api_pimpl;
	std::unique_ptr<ogl_render_api_pimpl> pimpl;
};

using ogl_render_api_uptr = std::unique_ptr<ogl_render_api>;
using ogl_render_api_sptr = std::shared_ptr<ogl_render_api>;
using ogl_render_api_wptr = std::weak_ptr<ogl_render_api>;

} // namespace render
} // namespace raptor_engine