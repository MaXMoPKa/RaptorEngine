#pragma once

#include <SDL_video.h>  // for SDL_Window

#include <memory>  // for unique_ptr

#include "defines.hpp"              // for u32
#include "structs/window_data.hpp"  // for structs, window_data

using namespace raptor_engine::structs;

namespace raptor_enigne {
namespace systems {

// TODO: think about SIMPL.

class hardware_system {
 public:
  hardware_system();
  hardware_system(u32 sdl_init_flags /*= SDL_INIT_VIDEO*/,
                  const window_data& window_info /*= window_info{}*/);

  hardware_system(hardware_system&& system) noexcept;
  hardware_system& operator=(hardware_system&& system) noexcept;

  hardware_system(const hardware_system&) = delete;
  hardware_system& operator=(const hardware_system&) = delete;

  ~hardware_system();

 public:
  void create(u32 sdl_init_flags = 20U /*SDL_INIT_VIDEO*/,
              const window_data& window_info = window_data{});

  void swap(hardware_system& system) noexcept;

  void reset() noexcept;

 public:
  [[nodiscard]] SDL_Window const* get_window() const noexcept;

  [[nodiscard]] u32 get_sdl_init_flags() const noexcept;

  [[nodiscard]] window_data const& get_window_data() const noexcept;

 private:
  class hardware_system_pimpl;
  std::unique_ptr<hardware_system_pimpl> pimpl;
};

}  // namespace systems
}  // namespace raptor_enigne