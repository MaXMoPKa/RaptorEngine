#pragma once

#include "structs/hardware_system_data.hpp"
#include "structs/scene_data.hpp"

namespace raptor_engine {
namespace structs {

struct init_engine_data {
public:
    init_engine_data();

    init_engine_data(const hardware_system_data_sptr& hardware_system_info_,
	                 const scene_data_sptr& scene_info_);
  
    ~init_engine_data();

public:
	const hardware_system_data_sptr get_hardware_system_info() const;

    const scene_data_sptr get_scene_info() const;

private:
    hardware_system_data_sptr hardware_system_info;
    scene_data_sptr		      scene_info;
};

using init_engine_data_uptr = std::unique_ptr<init_engine_data>;
using init_engine_data_sptr = std::shared_ptr<init_engine_data>;
using init_engine_data_wptr = std::weak_ptr<init_engine_data>;

}  // namespace structs
}  // namespace raptor_engine