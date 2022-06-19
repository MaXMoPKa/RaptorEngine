#include <gmock/gmock.h>

#include "systems/hardware_system.hpp"

using namespace testing;

TEST(hardware_system, create_with_default_constructor) {
  hardware_system hardware_system;
  ASSERT_THAT(hardware_system.get_window(), Eq(nullptr));
}

TEST(hardware_system, DISABLED_create_with_parametrized_constructor) {}

TEST(hardware_system, DISABLED_create_using_move_constructor) {}

TEST(hardware_system, DISABLED_create_with_move_assignment) {}

TEST(hardware_system, DISABLED_create_with_create_method) {}

TEST(hardware_system, DISABLED_swap_two_engine_instances_with_swap_method) {}

TEST(hardware_system, DISABLED_destroy_with_destroy_engine) {}
