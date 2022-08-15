#include <SDL2/SDL_video.h>  // for SDL_WINDOWPOS_CENTERED

#include <cstddef>     // for size_t
#include <functional>  // for function
#include <memory>      // for make_shared, __shared_pt...
#include <utility>     // for move

#include "core/engine.hpp"                   // for engine
#include "gmock/gmock.h"                     // for MakePredicateFormatterFr...
#include "gtest/gtest.h"                     // for Test, Eq, Message, TestP...
#include "structs/engine_data.hpp"           // for engine_data, engine_data...
#include "structs/hardware_system_data.hpp"  // for hardware_system_data
#include "structs/window_data.hpp"           // for window_data, window_data...
#include "systems/hardware_system.hpp"       // for hardware_system_sptr

using namespace testing;
using namespace raptor_engine;

class test_raptor_engine : public Test {
 public:
  static constexpr std::size_t WIDTH{1080U};
  static constexpr std::size_t HEIGHT{810U};
  static constexpr char TITLE[]{"Engine test window"};

 protected:
  void SetUp() {
    window_data_sptr window_info = std::make_shared<window_data>(
        TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
        SDL_WINDOW_OPENGL);

    hardware_system_data_sptr hardware_system_info =
        std::make_shared<hardware_system_data>(32U /*SDL_INIT_VIDEO*/,
                                               window_info);

    engine_data_sptr engine_info =
        std::make_shared<engine_data>(hardware_system_info);

    test_engine.create(engine_info);
  }

  void TearDown() {}

  engine test_engine;
};

TEST(engine, create_engine_instance_with_default_constructor) {
  engine engine{};

  ASSERT_THAT(engine.get_hardware_system()->get_window(), nullptr);
}

TEST(engine, create_engine_with_parametrized_constructor) {
  static const std::size_t WIDTH{1080U};
  static const std::size_t HEIGHT{810U};
  static const char TITLE[]{"Engine test window"};

  window_data_sptr window_info = std::make_shared<window_data>(
      TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
      SDL_WINDOW_OPENGL);

  hardware_system_data_sptr hardware_system_info =
      std::make_shared<hardware_system_data>(32U /*SDL_INIT_VIDEO*/,
                                             window_info);

  engine_data_sptr engine_info =
      std::make_shared<engine_data>(hardware_system_info);

  engine engine{engine_info};

  ASSERT_THAT(engine.get_hardware_system()->get_window_data()->width,
              Eq(WIDTH));
  ASSERT_THAT(engine.get_hardware_system()->get_window_data()->height,
              Eq(HEIGHT));
}

TEST(engine, create_engine_with_move_constructor) {
  static const std::size_t WIDTH{1080U};
  static const std::size_t HEIGHT{810U};
  static const char TITLE[]{"Engine test window"};
  std::function<engine()> test_function = [&]() -> engine {
    window_data_sptr window_info = std::make_shared<window_data>(
        TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
        SDL_WINDOW_OPENGL);

    hardware_system_data_sptr hardware_system_info =
        std::make_shared<hardware_system_data>(32U /*SDL_INIT_VIDEO*/,
                                               window_info);

    engine_data_sptr engine_info =
        std::make_shared<engine_data>(hardware_system_info);
    // std:move is used for prohibit RVO/NRVO
    return std::move(engine{engine_info});
  };

  engine engine = test_function();

  ASSERT_NE(engine.get_hardware_system()->get_window(), nullptr);
  ASSERT_THAT(engine.get_hardware_system()->get_window_data()->width,
              Eq(WIDTH));
  ASSERT_THAT(engine.get_hardware_system()->get_window_data()->height,
              Eq(HEIGHT));
}

TEST(engine, create_engine_with_move_assignment) {
  engine first_engine{};
  engine second_engine;

  second_engine = std::move(first_engine);

  ASSERT_NE(second_engine.get_hardware_system().get(), nullptr);
}

TEST(engine, create_with_create_method) {
  static const std::size_t WIDTH{1080U};
  static const std::size_t HEIGHT{810U};
  static const char TITLE[]{"Engine test window"};

  window_data_sptr window_info = std::make_shared<window_data>(
      TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
      SDL_WINDOW_OPENGL);

  hardware_system_data_sptr hardware_system_info =
      std::make_shared<hardware_system_data>(32U /*SDL_INIT_VIDEO*/,
                                             window_info);

  engine_data_sptr engine_info =
      std::make_shared<engine_data>(hardware_system_info);

  engine engine;
  engine.create(engine_info);

  ASSERT_THAT(engine.get_hardware_system()->get_window_data()->width,
              Eq(WIDTH));
  ASSERT_THAT(engine.get_hardware_system()->get_window_data()->height,
              Eq(HEIGHT));
}

TEST(engine, swap_two_engine_instances_with_swap_method) {
  engine first_engine{};

  static const std::size_t WIDTH{1080U};
  static const std::size_t HEIGHT{810U};
  static const char TITLE[]{"Engine test window"};

  window_data_sptr window_info = std::make_shared<window_data>(
      TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT,
      SDL_WINDOW_OPENGL);

  hardware_system_data_sptr hardware_system_info =
      std::make_shared<hardware_system_data>(32U /*SDL_INIT_VIDEO*/,
                                             window_info);

  engine_data_sptr engine_info =
      std::make_shared<engine_data>(hardware_system_info);

  engine second_engine;
  second_engine.create(engine_info);

  first_engine.swap(second_engine);

  ASSERT_THAT(first_engine.get_hardware_system()->get_window_data()->width,
              Eq(WIDTH));
  ASSERT_THAT(first_engine.get_hardware_system()->get_window_data()->height,
              Eq(HEIGHT));
  ASSERT_THAT(second_engine.get_hardware_system()->get_window_data()->width,
              Eq(0U));
  ASSERT_THAT(second_engine.get_hardware_system()->get_window_data()->height,
              Eq(0U));
}

TEST_F(test_raptor_engine, clear_with_reset_engine) {
  test_engine.reset();

  ASSERT_THAT(test_engine.get_hardware_system()->get_window_data()->width,
              Eq(0U));
  ASSERT_THAT(test_engine.get_hardware_system()->get_window_data()->height,
              Eq(0U));
}

TEST_F(test_raptor_engine, get_engine_data) {
  const auto engine_info = test_engine.get_engine_data();
  ASSERT_THAT(engine_info->hardware_system_info->window_info->width,
              Eq(test_raptor_engine::WIDTH));
  ASSERT_THAT(engine_info->hardware_system_info->window_info->height,
              Eq(test_raptor_engine::HEIGHT));
}