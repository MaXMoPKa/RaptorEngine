#include <string>   // for string
#include <utility>  // for swap, move

#include "hardware_system.hpp"

#include "SDL2/SDL.h"        // for SDL_Init, SDL_Quit, SDL_INIT_VIDEO
#include "SDL2/SDL_error.h"  // for SDL_GetError
#include "SDL2/SDL_log.h"    // for SDL_LogCritical, SDL_LOG_CATEGORY_ERROR

using namespace raptor_enigne::systems;

class hardware_system::hardware_system_pimpl {
 public:
  hardware_system_pimpl()
      : hardware_system_info{std::make_shared<hardware_system_data>()},
        window{nullptr}, gl_context {nullptr}
  {}

  hardware_system_pimpl(const hardware_system_data_sptr& hardware_system_info)
	  : hardware_system_info {hardware_system_info}, window {nullptr}, gl_context {nullptr}
  {
    init();
  }

  ~hardware_system_pimpl() {
    SDL_DestroyWindow(window.get());
    SDL_Quit();
  }

 public:
  void create(const hardware_system_data_sptr& hardware_system_info) {
    hardware_system_pimpl tmp{hardware_system_info};
    this->swap(tmp);
  }

  void swap(hardware_system_pimpl& pimpl) noexcept {
    if (this == &pimpl) {
      return;
    }
    std::swap(this->hardware_system_info, pimpl.hardware_system_info);
    std::swap(this->window, pimpl.window);
	std::swap(this->gl_context, pimpl.gl_context);
  }

  void reset() noexcept {
    hardware_system_pimpl tmp{};
    this->swap(tmp);
  }

 public:
  void process_events(bool& window_should_close) noexcept
  {
      SDL_Event event;
	  while (SDL_PollEvent(&event)) {
	      switch (event.type) {
		      case SDL_QUIT: {
		 	     window_should_close = true;
			     break;
		      }
		      case SDL_KEYDOWN: {
			      switch (event.key.keysym.sym) {
			          case SDLK_ESCAPE: {
				 	      window_should_close = true;
				 	      break;
				      }
			      }
			      break;
		      }
	      }
	  }
  }

 public:

  inline sdl_window_sptr get_window() const noexcept
  {
	  return this->window;
  }

  inline u32 get_sdl_init_flags() const noexcept {
    return this->hardware_system_info->sdl_init_flags;
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
    const auto result = SDL_Init(this->hardware_system_info->sdl_init_flags);

    if (result != 0) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Unable to initialize SDL: %s",
                      SDL_GetError());
      return;
    }
  }

  void create_window() {
	this->window = std::shared_ptr<SDL_Window>(SDL_CreateWindow(this->hardware_system_info->window_info->title.c_str(),
																 this->hardware_system_info->window_info->x_pos,
																 this->hardware_system_info->window_info->y_pos,
																 this->hardware_system_info->window_info->width,
																 this->hardware_system_info->window_info->height,
																 this->hardware_system_info->window_info->flags),
												                 SDL_DestroyWindow);

    if (this->window.get() == nullptr) {
      SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n",
                      SDL_GetError());
      return;
    }
  }

 private:
  sdl_window_sptr window;
  SDL_GLContext	gl_context;
  hardware_system_data_sptr hardware_system_info;
};

hardware_system::hardware_system()
    : pimpl{std::make_unique<hardware_system_pimpl>()} {}

hardware_system::hardware_system(
    const hardware_system_data_sptr& hardware_system_info)
    : pimpl(new hardware_system_pimpl(hardware_system_info)) {}

hardware_system::hardware_system(hardware_system&& system) noexcept = default;

hardware_system& hardware_system::operator=(hardware_system&& system) noexcept =
    default;

hardware_system::~hardware_system() = default;

void hardware_system::create(
    const hardware_system_data_sptr& hardware_system_info) {
  this->pimpl->create(hardware_system_info);
}

void hardware_system::swap(hardware_system& system) noexcept {
  this->pimpl.swap(system.pimpl);
}

void hardware_system::reset() noexcept { this->pimpl->reset(); }

void hardware_system::process_events(bool& window_should_close) noexcept
{
  this->pimpl->process_events(window_should_close);
}

sdl_window_sptr hardware_system::get_window() const noexcept
{
  return this->pimpl->get_window();
}
u32 hardware_system::get_sdl_init_flags() const noexcept {
  return this->pimpl->get_sdl_init_flags();
}
window_data_sptr hardware_system::get_window_data() const noexcept {
  return this->pimpl->get_window_data();
}
