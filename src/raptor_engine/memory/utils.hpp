#pragma once

#include "defines.hpp"

namespace raptor_engine {
namespace memory {
namespace utils {

static inline u8 GetAdjustment(const void* address_, u8 const alignment_)
{
	u8 adjustment = alignment_ - (reinterpret_cast<uiptr>(address_) & static_cast<uiptr>(alignment_ - 1));
    return adjustment == alignment_ ? 0 : adjustment;
}

}  // namespace utils
}  // namespace memory
}  // namespace raptor_engine