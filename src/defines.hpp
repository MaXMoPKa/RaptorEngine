#pragma once

#include <cstdint>
#include <memory>

#include "SDL_video.h"

using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using uiptr = uintptr_t;
using iptr	= intptr_t;

using sdl_window_uptr = std::unique_ptr<SDL_Window>;
using sdl_window_sptr = std::shared_ptr<SDL_Window>;
using sdl_window_wptr = std::weak_ptr<SDL_Window>;
