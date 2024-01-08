#pragma once

#include <cstddef>
#include <limits>

#define ENTITY_LUT_GROW	   1024
#define COMPONENT_LUT_GROW 1024

namespace raptor_engine {
namespace ecs {

using object_id = std::size_t;
using type_id   = std::size_t;

static const object_id INVALID_OBJECT_ID = std::numeric_limits<object_id>::max();


} // namespace ecs
} // namespace raptor_engine