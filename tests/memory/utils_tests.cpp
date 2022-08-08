#include <cstddef>  // for size_t
#include <memory>   // for allocator

#include "defines.hpp"       // for u8
#include "gmock/gmock.h"     // for MakePredicateFormatterFromMatcher, ASSER...
#include "gtest/gtest.h"     // for Eq, Test, Message, TestPartResult, TEST
#include "memory/utils.hpp"  // for get_adjustment, utils

using namespace testing;
using namespace raptor_engine::memory::utils;

TEST(utils,
     calculate_adjustment_for_different_alignments_and_allocation_sizes) {
  u8 const alignment = 8U;
  std::size_t allocation_size = 6;

  auto adjustment = get_adjustment(allocation_size, alignment);

  ASSERT_THAT(adjustment, Eq(alignment - allocation_size));

  allocation_size = 12;

  adjustment = get_adjustment(allocation_size, alignment);

  ASSERT_THAT(adjustment, Eq(allocation_size - alignment));

  allocation_size = 8;

  adjustment = get_adjustment(allocation_size, alignment);

  ASSERT_THAT(adjustment, Eq(alignment - allocation_size));
}