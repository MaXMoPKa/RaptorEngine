#include <string>   // for string
#include <utility>  // for swap, move

#include "base_hardware_system.hpp"

#include "SDL.h"        // for SDL_Init, SDL_Quit, SDL_INIT_VIDEO
#include "SDL_error.h"  // for SDL_GetError
#include "SDL_log.h"    // for SDL_LogCritical, SDL_LOG_CATEGORY_ERROR

using namespace raptor_enigne::systems;