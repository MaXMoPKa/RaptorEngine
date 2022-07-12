#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <utility>

#include "structs/window_data.hpp"

using namespace raptor_engine::structs;

namespace raptor_enigne {
namespace systems {

class hardware_system {
 public:
  hardware_system();
  hardware_system(u32 sdl_init_flags /*= SDL_INIT_VIDEO*/,
                  const window_data& window_info /*= window_info{}*/);

  hardware_system(hardware_system&& system) noexcept
      : sdl_init_flags{SDL_INIT_VIDEO}, window(nullptr), window_info{} {
    this->swap(system);
  }

  hardware_system& operator=(hardware_system&& system) noexcept;

  hardware_system(const hardware_system&) = delete;
  hardware_system& operator=(const hardware_system&) = delete;

  ~hardware_system();

 public:
  void create(u32 sdl_init_flags /*= SDL_INIT_VIDEO*/,
              const window_data& window_info);

  void swap(hardware_system& system) noexcept;

  void reset() noexcept;

 public:
  inline SDL_Window const* get_window() const noexcept { return this->window; }

  inline u32 get_sdl_init_flags() const noexcept {
    return this->sdl_init_flags;
  }

  inline window_data const& get_window_data() const noexcept {
    return this->window_info;
  }

 private:
  u32 sdl_init_flags;

  SDL_Window* window;
  window_data window_info;
};

}  // namespace systems
}  // namespace raptor_enigne