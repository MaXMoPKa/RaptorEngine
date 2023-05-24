#include "SDL.h"    // for SDL_INIT_VIDEO, SDL_INIT_AUDIO
#include "SDL_video.h"  // for SDL_WINDOW_OPENGL, SDL_WINDOW...

#include <cstddef>     // for size_t
#include <functional>  // for function
#include <memory>      // for allocator
#include <utility>     // for move

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "defines.hpp"                  // for u32
#include "structs/window_data.hpp"      // for window_data, window_data::WIN...
#include "systems/hardware_system.hpp"  // for hardware_system, systems

using namespace testing;
using namespace raptor_enigne::systems;
using namespace raptor_engine::structs;

TEST(hardware_system, create_with_default_constructor) {
  hardware_system hardware_system;
  ASSERT_THAT(hardware_system.get_window(), Eq(nullptr));
}

TEST(hardware_system, create_with_parametrized_constructor) {
  u32 sdl_init_flags{SDL_INIT_AUDIO | SDL_INIT_VIDEO};

  const char title[]{"Test title"};
  const auto x_pos{SDL_WINDOWPOS_UNDEFINED};
  const auto y_pos{SDL_WINDOWPOS_UNDEFINED};
  const std::size_t width{960};
  const std::size_t height{720};
  const auto flags{SDL_WINDOW_OPENGL | SDL_WINDOW_ALWAYS_ON_TOP};

  window_data window_info{title, x_pos, y_pos, width, height, flags};

  hardware_system hardware_system{sdl_init_flags, window_info};

  ASSERT_THAT(hardware_system.get_sdl_init_flags(), Eq(sdl_init_flags));
  ASSERT_NE(hardware_system.get_window(), nullptr);
  ASSERT_THAT(hardware_system.get_window_data().title, Eq(title));
  ASSERT_THAT(hardware_system.get_window_data().x_pos, Eq(x_pos));
  ASSERT_THAT(hardware_system.get_window_data().y_pos, Eq(y_pos));
  ASSERT_THAT(hardware_system.get_window_data().width, Eq(width));
  ASSERT_THAT(hardware_system.get_window_data().height, Eq(height));
  ASSERT_THAT(hardware_system.get_window_data().flags, Eq(flags));
}

TEST(hardware_system, create_using_move_constructor) {
  std::function<hardware_system(u32, const window_data&)> test_function =
      [](u32 sdl_init_flags,
         const window_data& window_info) -> hardware_system {
    // std:move is used for prohibit RVO/NRVO
    return std::move(hardware_system{sdl_init_flags, window_info});
  };

  u32 sdl_init_flags{SDL_INIT_AUDIO | SDL_INIT_VIDEO};

  const char title[]{"Test title"};
  const auto x_pos{SDL_WINDOWPOS_UNDEFINED};
  const auto y_pos{SDL_WINDOWPOS_UNDEFINED};
  const std::size_t width{960};
  const std::size_t height{720};
  const auto flags{SDL_WINDOW_OPENGL | SDL_WINDOW_ALWAYS_ON_TOP};

  window_data window_info{title, x_pos, y_pos, width, height, flags};

  hardware_system hardware_system = test_function(sdl_init_flags, window_info);

  ASSERT_NE(hardware_system.get_window(), nullptr);
  ASSERT_THAT(hardware_system.get_window_data().title, Eq(title));
  ASSERT_THAT(hardware_system.get_window_data().x_pos, Eq(x_pos));
  ASSERT_THAT(hardware_system.get_window_data().y_pos, Eq(y_pos));
  ASSERT_THAT(hardware_system.get_window_data().width, Eq(width));
  ASSERT_THAT(hardware_system.get_window_data().height, Eq(height));
  ASSERT_THAT(hardware_system.get_window_data().flags, Eq(flags));
}

TEST(hardware_system, create_with_move_assignment) {
  u32 sdl_init_flags{SDL_INIT_AUDIO | SDL_INIT_VIDEO};

  const char title[]{"Test title"};
  const auto x_pos{SDL_WINDOWPOS_UNDEFINED};
  const auto y_pos{SDL_WINDOWPOS_UNDEFINED};
  const std::size_t width{960};
  const std::size_t height{720};
  const auto flags{SDL_WINDOW_OPENGL | SDL_WINDOW_ALWAYS_ON_TOP};

  window_data window_info{title, x_pos, y_pos, width, height, flags};

  hardware_system first_system{sdl_init_flags, window_info};
  hardware_system second_system;

  second_system = std::move(first_system);

  ASSERT_NE(second_system.get_window(), nullptr);
  ASSERT_THAT(second_system.get_window_data().title, Eq(title));
  ASSERT_THAT(second_system.get_window_data().x_pos, Eq(x_pos));
  ASSERT_THAT(second_system.get_window_data().y_pos, Eq(y_pos));
  ASSERT_THAT(second_system.get_window_data().width, Eq(width));
  ASSERT_THAT(second_system.get_window_data().height, Eq(height));
  ASSERT_THAT(second_system.get_window_data().flags, Eq(flags));

  ASSERT_THAT(first_system.get_sdl_init_flags(), Eq(SDL_INIT_VIDEO));
  ASSERT_THAT(first_system.get_window(), Eq(nullptr));
  ASSERT_THAT(first_system.get_window_data().title,
              Eq(window_data::WINDOW_TITLE));
  ASSERT_THAT(first_system.get_window_data().x_pos, Eq(SDL_WINDOWPOS_CENTERED));
  ASSERT_THAT(first_system.get_window_data().y_pos, Eq(SDL_WINDOWPOS_CENTERED));
  ASSERT_THAT(first_system.get_window_data().width,
              Eq(window_data::WINDOW_PIXEL_WIDTH));
  ASSERT_THAT(first_system.get_window_data().height,
              Eq(window_data::WINDOW_PIXEL_HEIGHT));
  ASSERT_THAT(first_system.get_window_data().flags, Eq(SDL_WINDOW_OPENGL));
}

TEST(hardware_system, create_with_create_method) {
  hardware_system hardware_system;
  hardware_system.create(SDL_INIT_VIDEO, window_data{});

  ASSERT_THAT(hardware_system.get_sdl_init_flags(), Eq(SDL_INIT_VIDEO));
  ASSERT_NE(hardware_system.get_window(), nullptr);
  ASSERT_THAT(hardware_system.get_window_data().title,
              Eq(window_data::WINDOW_TITLE));
  ASSERT_THAT(hardware_system.get_window_data().x_pos,
              Eq(SDL_WINDOWPOS_CENTERED));
  ASSERT_THAT(hardware_system.get_window_data().y_pos,
              Eq(SDL_WINDOWPOS_CENTERED));
  ASSERT_THAT(hardware_system.get_window_data().width,
              Eq(window_data::WINDOW_PIXEL_WIDTH));
  ASSERT_THAT(hardware_system.get_window_data().height,
              Eq(window_data::WINDOW_PIXEL_HEIGHT));
  ASSERT_THAT(hardware_system.get_window_data().flags, Eq(SDL_WINDOW_OPENGL));
}

TEST(hardware_system, swap_two_hardware_system_instances_with_swap_method) {
  u32 sdl_init_flags{SDL_INIT_AUDIO | SDL_INIT_VIDEO};

  const char title[]{"Test title"};
  const auto x_pos{SDL_WINDOWPOS_UNDEFINED};
  const auto y_pos{SDL_WINDOWPOS_UNDEFINED};
  const std::size_t width{960};
  const std::size_t height{720};
  const auto flags{SDL_WINDOW_OPENGL | SDL_WINDOW_ALWAYS_ON_TOP};

  window_data window_info{title, x_pos, y_pos, width, height, flags};

  hardware_system first_system;
  first_system.create(sdl_init_flags, window_info);
  hardware_system second_system;
  first_system.swap(second_system);

  ASSERT_THAT(first_system.get_sdl_init_flags(), Eq(SDL_INIT_VIDEO));
  ASSERT_THAT(first_system.get_window(), Eq(nullptr));
  ASSERT_THAT(first_system.get_window_data().title,
              Eq(window_data::WINDOW_TITLE));
  ASSERT_THAT(first_system.get_window_data().x_pos, Eq(SDL_WINDOWPOS_CENTERED));
  ASSERT_THAT(first_system.get_window_data().y_pos, Eq(SDL_WINDOWPOS_CENTERED));
  ASSERT_THAT(first_system.get_window_data().width,
              Eq(window_data::WINDOW_PIXEL_WIDTH));
  ASSERT_THAT(first_system.get_window_data().height,
              Eq(window_data::WINDOW_PIXEL_HEIGHT));
  ASSERT_THAT(first_system.get_window_data().flags, Eq(SDL_WINDOW_OPENGL));

  ASSERT_THAT(second_system.get_sdl_init_flags(), Eq(sdl_init_flags));
  ASSERT_NE(second_system.get_window(), nullptr);
  ASSERT_THAT(second_system.get_window_data().title, Eq(title));
  ASSERT_THAT(second_system.get_window_data().x_pos, Eq(x_pos));
  ASSERT_THAT(second_system.get_window_data().y_pos, Eq(y_pos));
  ASSERT_THAT(second_system.get_window_data().width, Eq(width));
  ASSERT_THAT(second_system.get_window_data().height, Eq(height));
  ASSERT_THAT(second_system.get_window_data().flags, Eq(flags));
}

TEST(hardware_system, clear_with_reset_engine) {
  u32 sdl_init_flags{SDL_INIT_AUDIO | SDL_INIT_VIDEO};

  const char title[]{"Test title"};
  const auto x_pos{SDL_WINDOWPOS_UNDEFINED};
  const auto y_pos{SDL_WINDOWPOS_UNDEFINED};
  const std::size_t width{960};
  const std::size_t height{720};
  const auto flags{SDL_WINDOW_OPENGL | SDL_WINDOW_ALWAYS_ON_TOP};

  window_data window_info{title, x_pos, y_pos, width, height, flags};

  hardware_system hardware_system;
  hardware_system.create(sdl_init_flags, window_info);
  hardware_system.reset();

  ASSERT_THAT(hardware_system.get_sdl_init_flags(), Eq(SDL_INIT_VIDEO));
  ASSERT_THAT(hardware_system.get_window(), Eq(nullptr));
  ASSERT_THAT(hardware_system.get_window_data().title,
              Eq(window_data::WINDOW_TITLE));
  ASSERT_THAT(hardware_system.get_window_data().x_pos,
              Eq(SDL_WINDOWPOS_CENTERED));
  ASSERT_THAT(hardware_system.get_window_data().y_pos,
              Eq(SDL_WINDOWPOS_CENTERED));
  ASSERT_THAT(hardware_system.get_window_data().width,
              Eq(window_data::WINDOW_PIXEL_WIDTH));
  ASSERT_THAT(hardware_system.get_window_data().height,
              Eq(window_data::WINDOW_PIXEL_HEIGHT));
  ASSERT_THAT(hardware_system.get_window_data().flags, Eq(SDL_WINDOW_OPENGL));
}
