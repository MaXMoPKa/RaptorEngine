#pragma once

#include <memory>
#include <string>  // for allocator, string

#include "SDL_video.h"  // for SDL_WINDOWPOS_CENTERED, SDL_WINDOW_OPENGL
#include "defines.hpp"  // for u32

namespace raptor_engine {
namespace structs {

struct window_data {
  static constexpr u16 WINDOW_PIXEL_WIDTH{640};
  static constexpr u16 WINDOW_PIXEL_HEIGHT{480};
  static constexpr char WINDOW_TITLE[] = "Raptor engine window";

 public:
  window_data();

  window_data(const std::string& title /*= WINDOW_TITLE*/,
              int x_pos /*= SDL_WINDOWPOS_CENTERED*/,
              int y_pos /*= SDL_WINDOWPOS_CENTERED*/,
              int width /*= WINDOW_PIXEL_WIDTH*/,
              int height /*= WINDOW_PIXEL_HEIGHT*/,
              u32 flags /*= SDL_WINDOW_OPENGL*/);

  ~window_data() = default;

 public:
  std::string title;
  int x_pos;
  int y_pos;
  int width;
  int height;
  u32 flags;
};

using window_data_uptr = std::unique_ptr<window_data>;
using window_data_sptr = std::shared_ptr<window_data>;
using window_data_wptr = std::weak_ptr<window_data>;

void swap(window_data_sptr& first_data, window_data_sptr& second_data);

}  // namespace structs
}  // namespace raptor_engine
