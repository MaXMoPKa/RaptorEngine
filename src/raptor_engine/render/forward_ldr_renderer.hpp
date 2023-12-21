#pragma once

#include "render/base_renderer.hpp"

using namespace raptor_engine::structs;

namespace raptor_engine {
namespace render {

class forward_ldr_renderer : public base_renderer
{
public:
	forward_ldr_renderer();

	forward_ldr_renderer(const high_level_renderer_data_sptr& renderer_data, const scene_data_sptr& scene_info);

	forward_ldr_renderer(forward_ldr_renderer&& renderer) noexcept;
	forward_ldr_renderer& operator=(forward_ldr_renderer&& renderer) noexcept;

	forward_ldr_renderer(const forward_ldr_renderer&) = delete;
	forward_ldr_renderer& operator=(const forward_ldr_renderer&) = delete;

	~forward_ldr_renderer() override;

public:
	void pre_update() override;

	void update(const std::vector<renderable_object_sptr>& renderable_objects_) override;

	void post_update() override;

public:

	void create(const high_level_renderer_data_sptr& renderer_data, const scene_data_sptr& scene_info) override;

	void swap(base_renderer& renderer) noexcept;

	void reset() noexcept;
};

using forward_ldr_renderer_uptr = std::unique_ptr<forward_ldr_renderer>;
using forward_ldr_renderer_sptr = std::shared_ptr<forward_ldr_renderer>;
using forward_ldr_renderer_wptr = std::weak_ptr<forward_ldr_renderer>;

}
}
