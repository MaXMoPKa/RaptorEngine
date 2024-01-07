#pragma once

#include "structs/window_data.hpp"

namespace raptor_engine {
namespace structs {

struct hardware_system_data {
 public:
  hardware_system_data();

  hardware_system_data(u32 sdl_init_flags /*SDL_INIT_VIDEO*/,
                       const window_data_sptr& window_info);

  ~hardware_system_data() = default;

 public:
  const window_data_sptr get_window_data() const;

  u32 get_init_flags() const;

 private:
  window_data_sptr window_info;
  u32 sdl_init_flags;
};

using hardware_system_data_uptr = std::unique_ptr<hardware_system_data>;
using hardware_system_data_sptr = std::shared_ptr<hardware_system_data>;
using hardware_system_data_wptr = std::weak_ptr<hardware_system_data>;

}  // namespace structs
}  // namespace raptor_engine