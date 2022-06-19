#pragma once

#include <SDL2/SDL.h>

class hardware_system {
 public:
  hardware_system() : window{nullptr} {}

 public:
  SDL_Window const* get_window() const noexcept { return window; }

 private:
  SDL_Window* window;
};