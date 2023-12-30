#pragma once 

#include "structs/init_render_engine_data.hpp"

#include "render/base_renderer.hpp"

#include "render/renderable_object.hpp"

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

	void update(u64 time_);

	void post_update();

public:

	void set_renderable_objects(const std::vector<renderable_object_sptr>& renderable_objects_);

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