#include "SDL2/SDL_video.h"
#include "defines.hpp"

struct window_data {
  static constexpr std::size_t WINDOW_PIXEL_WIDTH{640};
  static constexpr std::size_t WINDOW_PIXEL_HEIGHT{480};
  static constexpr char WINDOW_TITLE[]{"Raptor engine window"};

 public:
  window_data()
      : title{WINDOW_TITLE},
        x_pos{SDL_WINDOWPOS_CENTERED},
        y_pos{SDL_WINDOWPOS_CENTERED},
        width{WINDOW_PIXEL_WIDTH},
        height{WINDOW_PIXEL_HEIGHT},
        flags{SDL_WINDOW_OPENGL} {}

  window_data(const std::string& title /*= WINDOW_TITLE*/,
              int x_pos /*= SDL_WINDOWPOS_CENTERED*/,
              int y_pos /*= SDL_WINDOWPOS_CENTERED*/,
              int width /*= WINDOW_PIXEL_WIDTH*/,
              int height /*= WINDOW_PIXEL_HEIGHT*/,
              u32 flags /*= SDL_WINDOW_OPENGL*/)
      : title{title},
        x_pos{x_pos},
        y_pos{y_pos},
        width{width},
        height{height},
        flags{flags} {}

  ~window_data() = default;

 public:
  std::string title;
  int x_pos;
  int y_pos;
  int width;
  int height;
  u32 flags;
};

void swap(window_data& first_data, window_data& second_data) {
  if (&first_data == &second_data) {
    return;
  }
  std::swap(first_data.title, second_data.title);
  std::swap(first_data.x_pos, second_data.x_pos);
  std::swap(first_data.y_pos, second_data.y_pos);
  std::swap(first_data.width, second_data.width);
  std::swap(first_data.height, second_data.height);
  std::swap(first_data.flags, second_data.flags);
}
