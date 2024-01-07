#pragma once

#include "structs/high_level_renderer_data.hpp"
#include "structs/scene_data.hpp"

namespace raptor_engine {
namespace structs {

enum class high_level_renderer_type : u8
{
	// Realized
	FORWARD_LDR_RENDERER = 0,

	// Unrealized
	FORWARD_HDR_RENDERER,
	DEFFERED_LDR_RENDERER,
	DEFERRED_HDR_RENDERER,

	HIGHT_LEVEL_RENDERER_COUNT
};

struct init_render_engine_data {
public:
	init_render_engine_data();

	init_render_engine_data(const high_level_renderer_data_sptr& renderer_info_,
					   const high_level_renderer_type& renderer_type_ = high_level_renderer_type::FORWARD_LDR_RENDERER,
					   const scene_data_sptr& scene_info_ = nullptr);
	
	~init_render_engine_data();

public:
	const high_level_renderer_data_sptr get_renderer_info() const;

	high_level_renderer_type get_renderer_type() const;

	const scene_data_sptr get_scene_info() const;

private:
  high_level_renderer_data_sptr renderer_info;
  high_level_renderer_type		renderer_type;
  scene_data_sptr				scene_info;
};

using init_render_engine_data_uptr = std::unique_ptr<init_render_engine_data>;
using init_render_engine_data_sptr = std::shared_ptr<init_render_engine_data>;
using init_render_engine_data_wptr = std::weak_ptr<init_render_engine_data>;

}  // namespace structs
}  // namespace raptor_engine