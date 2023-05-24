#include "gtest/gtest.h" // for Test, TestInfo (ptr only), TEST, testing

#include <memory>  // for allocator

#include "core/engine.hpp"  // for engine, raptor_engine

using namespace testing;
using namespace raptor_engine;

TEST(raptor_engine, create_engine_instance_with_default_constructor) {
  engine engine{};
}