#pragma once

#include "structs/render_object_data.hpp"
#include "render/draw_config.hpp"

namespace raptor_engine {
namespace structs {

struct scene_data
{
public:
	scene_data();

	scene_data(const render_objects& render_objects_, const draw_config_sptr& draw_config_);
	
	~scene_data();

public:
	const render_objects& get_objects() const;

	const draw_config_sptr get_draw_config() const;

private:
	class scene_data_pimpl;
	std::unique_ptr<scene_data_pimpl> pimpl;
};

using scene_data_uptr = std::unique_ptr<scene_data>;
using scene_data_sptr = std::shared_ptr<scene_data>;
using scene_data_wptr = std::weak_ptr<scene_data>;

}  // namespace structs
}  // namespace raptor_engine