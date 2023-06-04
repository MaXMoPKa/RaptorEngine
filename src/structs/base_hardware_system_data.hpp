#pragma once

#include "structs/window_data.hpp"

namespace raptor_engine {
namespace structs {

struct base_hardware_system_data {
 public:

	base_hardware_system_data();
	base_hardware_system_data(u32 init_flags /*SDL_INIT_VIDEO*/,
                              const window_data_sptr& window_info /*= std::make_shared<window_data>()*/);
	~base_hardware_system_data() = default;

 public:
  window_data_sptr window_info{};
  u32 init_flags{};
};

using base_hardware_system_data_uptr = std::unique_ptr<base_hardware_system_data>;
using base_hardware_system_data_sptr = std::shared_ptr<base_hardware_system_data>;
using base_hardware_system_data_wptr = std::weak_ptr<base_hardware_system_data>;

void swap(base_hardware_system_data_sptr& first_data, base_hardware_system_data_sptr& second_data);

}  // namespace structs
}  // namespace raptor_engine