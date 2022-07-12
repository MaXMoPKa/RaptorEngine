#pragma once

#include <cstddef>  // for size_t
#include <string>   // for allocator, string

#include "SDL2/SDL_video.h"  // for SDL_WINDOWPOS_CENTERED, SDL_WINDOW_OPENGL
#include "defines.hpp"       // for u32

namespace raptor_engine {
namespace structs {

struct window_data {
  static constexpr std::size_t WINDOW_PIXEL_WIDTH{640};
  static constexpr std::size_t WINDOW_PIXEL_HEIGHT{480};
  static constexpr char WINDOW_TITLE[]{"Raptor engine window"};

 public:
  window_data(const std::string& title = WINDOW_TITLE,
              int x_pos = SDL_WINDOWPOS_CENTERED,
              int y_pos = SDL_WINDOWPOS_CENTERED,
              int width = WINDOW_PIXEL_WIDTH, int height = WINDOW_PIXEL_HEIGHT,
              u32 flags = SDL_WINDOW_OPENGL);

  ~window_data() = default;

 public:
  std::string title;
  int x_pos;
  int y_pos;
  int width;
  int height;
  u32 flags;
};

void swap(window_data& first_data, window_data& second_data);

}  // namespace structs
}  // namespace raptor_engine
