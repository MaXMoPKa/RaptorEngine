#pragma once

#include <cstddef>
#include <limits>

#define ENTITY_LUT_GROW	   1024
#define COMPONENT_LUT_GROW 1024
#define COMPONENT_T_CHUNK_SIZE 512

namespace raptor_engine {
namespace ecs {

using object_id = std::size_t;
using type_id   = std::size_t;

static const object_id INVALID_OBJECT_ID = std::numeric_limits<object_id>::max();


} // namespace ecs
} // namespace raptor_engine