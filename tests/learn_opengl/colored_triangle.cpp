#include <cstddef>     // for size_t
#include <functional>  // for function
#include <memory>      // for __shared_ptr_access, mak...
#include <utility>     // for move

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "SDL.h"    // for SDL_INIT_VIDEO, SDL_INIT_AUDIO
#include "SDL_video.h"  // for SDL_WINDOW_OPENGL, SDL_WINDOW...

#include "glad/glad.h"

#include "defines.hpp"                  // for u32
#include "structs/window_data.hpp"      // for window_data, window_data::WIN...
#include "core/engine.hpp"  // for hardware_system, systems

using namespace testing;
using namespace raptor_enigne::systems;
using namespace raptor_engine::structs;
using namespace raptor_engine;

TEST(learn_opengl, colored_triangle)
{
  static const std::size_t WIDTH {1080U};
  static const std::size_t HEIGHT {810U};
  static const char		   TITLE[] {"Engine test window"};

  window_data_sptr window_info = std::make_shared<window_data>(TITLE,
															   SDL_WINDOWPOS_CENTERED,
															   SDL_WINDOWPOS_CENTERED,
															   WIDTH,
															   HEIGHT,
															   SDL_WINDOW_OPENGL);

  hardware_system_data_sptr hardware_system_info =
	  std::make_shared<hardware_system_data>(32U, window_info);

  engine_data_sptr engine_info = std::make_shared<engine_data>(hardware_system_info);

  engine engine{engine_info};

  engine.run();
}