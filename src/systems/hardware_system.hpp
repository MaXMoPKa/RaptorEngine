#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <utility>

#include "defines.hpp"
#include "structs/window_data.hpp"

class hardware_system {
 public:
  hardware_system()
      : sdl_init_flags{SDL_INIT_VIDEO}, window{nullptr}, window_info{} {}

  hardware_system(u32 sdl_init_flags /*= SDL_INIT_VIDEO*/,
                  const window_data& window_info)
      : sdl_init_flags{sdl_init_flags},
        window{nullptr},
        window_info{window_info} {
    const auto result = SDL_Init(this->sdl_init_flags);
    if (result != 0) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s",
                      SDL_GetError());
      return;
    }

    this->window = SDL_CreateWindow(
        this->window_info.title.c_str(), this->window_info.x_pos,
        this->window_info.y_pos, this->window_info.width,
        this->window_info.height, this->window_info.flags);

    if (this->window == nullptr) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n",
                      SDL_GetError());
      return;
    }
  }

  hardware_system(hardware_system&& system) noexcept
      : sdl_init_flags{SDL_INIT_VIDEO}, window(nullptr), window_info{} {
    this->swap(system);
  }

  hardware_system& operator=(hardware_system&& system) noexcept {
    hardware_system tmp{std::move(system)};
    this->swap(tmp);
    return *this;
  }

  hardware_system(const hardware_system&) = delete;
  hardware_system& operator=(const hardware_system&) = delete;

  ~hardware_system() {
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

 public:
  void create(u32 sdl_init_flags /*= SDL_INIT_VIDEO*/,
              const window_data& window_info) {
    hardware_system tmp{sdl_init_flags, window_info};
    this->swap(tmp);
  }

  void swap(hardware_system& system) noexcept {
    if (this == &system) {
      return;
    }

    std::swap(this->sdl_init_flags, system.sdl_init_flags);
    std::swap(this->window, system.window);
    std::swap(this->window_info, system.window_info);
  }

  void reset() noexcept {
    hardware_system tmp{};
    this->swap(tmp);
  }

 public:
  SDL_Window const* get_window() const noexcept { return this->window; }

  u32 get_sdl_init_flags() const noexcept { return this->sdl_init_flags; }

  window_data const& get_window_data() const noexcept {
    return this->window_info;
  }

 private:
  u32 sdl_init_flags;

  SDL_Window* window;
  window_data window_info;
};