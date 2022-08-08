#include <memory>  // for allocator

#include "core/engine.hpp"          // for engine
#include "gtest/gtest.h"            // for Message, TestPartResult
#include "structs/window_data.hpp"  // for raptor_engine
#include "utils/types.hpp"          // for hardware_system_sptr

using namespace testing;
using namespace raptor_engine;

TEST(raptor_engine, create_engine_instance_with_default_constructor) {
  engine engine{};

  ASSERT_NE(engine.get_hardware_system().get(), nullptr);
}