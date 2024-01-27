#pragma once

#include "defines.hpp"

namespace raptor_engine {
namespace memory {
namespace utils {

static constexpr u8 GetAdjustment(u64 const size_, u8 const alignment_) {
  u8 adjustment = alignment_ - (size_ & (alignment_ - 1));
  return adjustment == alignment_ ? 0 : adjustment;
}

}  // namespace utils
}  // namespace memory
}  // namespace raptor_engine