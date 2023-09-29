#pragma once 

#include "structs/init_render_engine_data.hpp"

#include "render/base_renderer.hpp"

using namespace raptor_engine::structs;

namespace raptor_engine {
namespace render {

class render_engine
{
public:
	render_engine();

	render_engine(const init_render_engine_data_sptr& init_render_engine_data);

	render_engine(render_engine&& renderer) noexcept;
	render_engine& operator=(render_engine&& renderer) noexcept;

	render_engine(const render_engine&)			   = delete;
	render_engine& operator=(const render_engine&) = delete;

	~render_engine();

public:
	void create(const init_render_engine_data_sptr& init_render_engine_data);

	void swap(render_engine& render_eng) noexcept;

	void reset() noexcept;

public:
	void pre_update();

	void update();

	void post_update();

public:
	[[nodiscard]] base_renderer_sptr get_renderer() const;

private:
	class render_engine_pimpl;
	std::unique_ptr<render_engine_pimpl> pimpl;
};

using render_engine_uptr = std::unique_ptr<render_engine>;
using render_engine_sptr = std::shared_ptr<render_engine>;
using render_engine_wptr = std::weak_ptr<render_engine>;

}
}