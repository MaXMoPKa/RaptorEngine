#include <gmock/gmock.h>

#include "core/engine.hpp"

using namespace testing;
using namespace raptor_engine;

TEST(raptor_engine, create_engine_instance_with_default_constructor) {
  engine engine{};
}