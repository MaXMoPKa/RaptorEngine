#pragma once

#include "defines.hpp"

namespace raptor_engine {
namespace memory {
    namespace utils {

        static constexpr u8 get_adjustment(std::size_t const size, u8 const alignment)
        {
            u8 adjustment = alignment - (size & (alignment - 1));
            return adjustment == alignment ? 0 : adjustment;
        }

    } // namespace utils
} // namespace memory
} // namespace raptor_engine