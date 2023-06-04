#include <string>   // for string
#include <utility>  // for swap, move

#include "sdl_hardware_system.hpp"

#include "SDL.h"        // for SDL_Init, SDL_Quit, SDL_INIT_VIDEO
#include "SDL_error.h"  // for SDL_GetError
#include "SDL_log.h"    // for SDL_LogCritical, SDL_LOG_CATEGORY_ERROR

using namespace raptor_enigne::systems;

class sdl_hardware_system::sdl_hardware_system_pimpl
{
 public:

	sdl_hardware_system_pimpl()
      : hardware_system_info{std::make_shared<sdl_hardware_system_data>()},
        window{nullptr}, gl_context{nullptr} { }

  sdl_hardware_system_pimpl(const sdl_hardware_system_data_sptr& hardware_system_info)
	  : hardware_system_info {hardware_system_info}, window {nullptr}, gl_context {nullptr}
  {
    init();
  }

  ~sdl_hardware_system_pimpl()
  {
	SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

 public:
  void create(const sdl_hardware_system_data_sptr& hardware_system_info) {
	sdl_hardware_system_pimpl tmp {hardware_system_info};
    this->swap(tmp);
  }

  void swap(sdl_hardware_system_pimpl& pimpl) noexcept
  {
    if (this == &pimpl) {
      return;
    }
    std::swap(this->hardware_system_info, pimpl.hardware_system_info);
    std::swap(this->window, pimpl.window);
	std::swap(this->gl_context, pimpl.gl_context);
  }

  void reset() noexcept {
	sdl_hardware_system_pimpl tmp {};
    this->swap(tmp);
  }

 public:

  inline SDL_Window const* get_window() const noexcept { return this->window; }

  inline u32 get_sdl_init_flags() const noexcept {
    return this->hardware_system_info->init_flags;
  }

  inline window_data_sptr get_window_data() const noexcept {
    return this->hardware_system_info->window_info;
  }

 private:
  void init() {
    this->init_sdl();
    this->create_window();
  }

  void init_sdl() {
    const auto result = SDL_Init(this->hardware_system_info->init_flags);

    if (result != 0) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s",
                      SDL_GetError());
      return;
    }
  }

  void create_window() {
    this->window =
        SDL_CreateWindow(this->hardware_system_info->window_info->title.c_str(),
                         this->hardware_system_info->window_info->x_pos,
                         this->hardware_system_info->window_info->y_pos,
                         this->hardware_system_info->window_info->width,
                         this->hardware_system_info->window_info->height,
                         this->hardware_system_info->window_info->flags);

    if (this->window == nullptr) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n",
                      SDL_GetError());
      return;
    }

    gl_context = SDL_GL_CreateContext(this->window);
  }

 private:
  SDL_Window* window;
  SDL_GLContext gl_context;
  sdl_hardware_system_data_sptr hardware_system_info;
};

sdl_hardware_system::sdl_hardware_system()
    : pimpl{std::make_unique<sdl_hardware_system_pimpl>()} {}

sdl_hardware_system::sdl_hardware_system(
    const sdl_hardware_system_data_sptr& hardware_system_info)
    : pimpl(new sdl_hardware_system_pimpl(hardware_system_info)) {}

sdl_hardware_system::sdl_hardware_system(sdl_hardware_system&& system) noexcept = default;

sdl_hardware_system& sdl_hardware_system::operator=(sdl_hardware_system&& system) noexcept =
    default;

sdl_hardware_system::~sdl_hardware_system() = default;

void sdl_hardware_system::create(
    const sdl_hardware_system_data_sptr& hardware_system_info) {
  this->pimpl->create(hardware_system_info);
}

void sdl_hardware_system::swap(sdl_hardware_system& system) noexcept
{
  this->pimpl.swap(system.pimpl);
}

void sdl_hardware_system::reset() noexcept
{
  this->pimpl->reset();
}

SDL_Window const* sdl_hardware_system::get_window() const noexcept
{
  return this->pimpl->get_window();
}
u32 sdl_hardware_system::get_sdl_init_flags() const noexcept
{
  return this->pimpl->get_sdl_init_flags();
}
window_data_sptr sdl_hardware_system::get_window_data() const noexcept
{
  return this->pimpl->get_window_data();
}
