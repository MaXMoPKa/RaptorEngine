#include "hardware_system.hpp"

#include <SDL2/SDL.h>        // for SDL_Init, SDL_Quit, SDL_INIT_VIDEO
#include <SDL2/SDL_error.h>  // for SDL_GetError
#include <SDL2/SDL_log.h>    // for SDL_LogCritical, SDL_LOG_CATEGORY_ERROR

#include <string>   // for string
#include <utility>  // for swap, move

using namespace raptor_enigne::systems;

class hardware_system::hardware_system_pimpl {
 public:
  hardware_system_pimpl()
      : sdl_init_flags{SDL_INIT_VIDEO}, window{nullptr}, window_info{} {}

  hardware_system_pimpl(u32 sdl_init_flags /*= SDL_INIT_VIDEO*/,
                        const window_data& window_info /*=window_data{}*/)
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

  ~hardware_system_pimpl() {
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

 public:
  void create(u32 sdl_init_flags /*= SDL_INIT_VIDEO*/,
              const window_data& window_info) {
    hardware_system_pimpl tmp{sdl_init_flags, window_info};
    this->swap(tmp);
  }

  void swap(hardware_system_pimpl& pimpl) noexcept {
    if (this == &pimpl) {
      return;
    }
    std::swap(this->sdl_init_flags, pimpl.sdl_init_flags);
    std::swap(this->window, pimpl.window);
    std::swap(this->window_info, pimpl.window_info);
  }

  void reset() noexcept {
    hardware_system_pimpl tmp{};
    this->swap(tmp);
  }

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

hardware_system::hardware_system() : pimpl(new hardware_system_pimpl()) {}

hardware_system::hardware_system(u32 sdl_init_flags,
                                 const window_data& window_info)
    : pimpl(new hardware_system_pimpl(sdl_init_flags, window_info)) {}

hardware_system::hardware_system(hardware_system&& system) noexcept = default;

hardware_system& hardware_system::operator=(hardware_system&& system) noexcept =
    default;

hardware_system::~hardware_system() = default;

void hardware_system::create(u32 sdl_init_flags,
                             const window_data& window_info) {
  this->pimpl->create(sdl_init_flags, window_info);
}

void hardware_system::swap(hardware_system& system) noexcept {
  this->pimpl->swap(*system.pimpl);
}

void hardware_system::reset() noexcept { this->pimpl->reset(); }

SDL_Window const* hardware_system::get_window() const noexcept {
  return this->pimpl->get_window();
}
u32 hardware_system::get_sdl_init_flags() const noexcept {
  return this->pimpl->get_sdl_init_flags();
}
window_data const& hardware_system::get_window_data() const noexcept {
  return this->pimpl->get_window_data();
}
