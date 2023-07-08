#pragma once

#include "structs/high_level_renderer_data.hpp"

#include "render/ogl_render_api.hpp"

using namespace raptor_engine::structs;

namespace raptor_engine {
namespace render { 

class base_renderer
{
public:
	base_renderer();

	base_renderer(const high_level_renderer_data_sptr& renderer_data);

	base_renderer(base_renderer&& renderer) noexcept;
	base_renderer& operator=(base_renderer&& renderer) noexcept;

	base_renderer(const base_renderer&)			   = delete;
	base_renderer& operator=(const base_renderer&) = delete;

	virtual ~base_renderer();

public:
	virtual void create(const high_level_renderer_data_sptr& renderer_data);

	virtual void swap(base_renderer& renderer) noexcept;

	virtual void reset() noexcept;

public:
	virtual void pre_update();

	virtual void update();

	virtual void post_update();

public:
	[[nodiscard]] base_render_api_sptr get_render_api() const;

private:
	class base_renderer_pimpl;
	std::unique_ptr<base_renderer_pimpl> pimpl;
};

using base_renderer_uptr = std::unique_ptr<base_renderer>;
using base_renderer_sptr = std::shared_ptr<base_renderer>;
using base_renderer_wptr = std::weak_ptr<base_renderer>;

}
}