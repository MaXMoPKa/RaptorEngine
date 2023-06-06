#pragma once

#include "render/base_render_api.hpp"

namespace raptor_engine {
namespace render { 

class base_renderer
{
public:

	base_renderer() { }

	base_renderer(base_render_api_sptr& render_api_ptr) : render_api {render_api_ptr} { }

	base_renderer(base_renderer&& renderer) noexcept            = default;
	base_renderer& operator=(base_renderer&& renderer) noexcept = default;

	base_renderer(const base_renderer&)			   = delete;
	base_renderer& operator=(const base_renderer&) = delete;

	virtual ~base_renderer() { }
public:

	virtual void create(base_render_api_sptr& render_api_ptr)
	{
		base_renderer tmp {};
		this->swap(tmp);
	}

	virtual void swap(base_renderer& api) noexcept
	{
		if (this == &api) {
			return;
		}

		std::swap(this->render_api, api.render_api);
	}

	virtual void reset() noexcept
	{
		base_renderer tmp {};
		this->swap(tmp);
	}

private:
	base_render_api_sptr render_api;
};

using base_renderer_uptr = std::unique_ptr<base_renderer>;
using base_renderer_sptr = std::shared_ptr<base_renderer>;
using base_renderer_wptr = std::weak_ptr<base_renderer>;

}
}