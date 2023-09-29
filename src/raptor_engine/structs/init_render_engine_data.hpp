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

class init_render_engine_data {
 public:
	init_render_engine_data();

	init_render_engine_data(const high_level_renderer_data_sptr& renderer_info,
					   const high_level_renderer_type& renderer_type = high_level_renderer_type::FORWARD_LDR_RENDERER,
					   const scene_data_sptr& scene_data = nullptr);
	
	~init_render_engine_data();

 public:
  high_level_renderer_data_sptr renderer_data;
  high_level_renderer_type		high_level_renderer;
  scene_data_sptr				scene_data;
};

using init_render_engine_data_uptr = std::unique_ptr<init_render_engine_data>;
using init_render_engine_data_sptr = std::shared_ptr<init_render_engine_data>;
using init_render_engine_data_wptr = std::weak_ptr<init_render_engine_data>;

}  // namespace structs
}  // namespace raptor_engine