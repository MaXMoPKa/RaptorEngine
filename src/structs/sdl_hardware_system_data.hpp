#pragma once

#include "structs/base_hardware_system_data.hpp"

namespace raptor_engine {
namespace structs {

struct sdl_hardware_system_data : base_hardware_system_data {
 public:

	sdl_hardware_system_data();
	sdl_hardware_system_data(u32 sdl_init_flags /*SDL_INIT_VIDEO*/,
                             const window_data_sptr& window_info /*= std::make_shared<window_data>()*/);
	~sdl_hardware_system_data() = default;
};

using sdl_hardware_system_data_uptr = std::unique_ptr<sdl_hardware_system_data>;
using sdl_hardware_system_data_sptr = std::shared_ptr<sdl_hardware_system_data>;
using sdl_hardware_system_data_wptr = std::weak_ptr<sdl_hardware_system_data>;

void swap(sdl_hardware_system_data_sptr& first_data, sdl_hardware_system_data_sptr& second_data);

}  // namespace structs
}  // namespace raptor_engine