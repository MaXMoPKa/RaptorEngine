#pragma once

#include <cstddef>
#include <limits>

#include "defines.hpp"

#define ENTITY_LUT_GROW	              1024
#define ENTITY_T_CHUNK_SIZE	          512
#define COMPONENT_LUT_GROW            1024
#define COMPONENT_T_CHUNK_SIZE        512
#define ECS_EVENT_MEMORY_BUFFER_SIZE  4194304 // 4MB
#define ECS_SYSTEM_MEMORY_BUFFER_SIZE 8388608 // 8MB

namespace raptor_engine {
namespace ecs {

using ObjectId = std::size_t;
using TypeId   = std::size_t;

static const ObjectId INVALID_OBJECT_ID = std::numeric_limits<ObjectId>::max();
static const TypeId	  INVALID_TYPE_ID	= std::numeric_limits<TypeId>::max();

union TimeStamp
{
	f32 asFloat;
	u32 asUint;

	TimeStamp() 
		: asUint {0}
	{ }

	explicit TimeStamp(f32 value_) 
		: asFloat {value_}
	{ }

	explicit TimeStamp(u32 value_) 
		: asUint {value_} 
	{ }

	operator u32() const
	{
		return this->asUint;
	}

	inline const bool operator==(const TimeStamp& other_) const
	{
		return this->asUint == other_.asUint;
	}
	inline const bool operator!=(const TimeStamp& other_) const
	{
		return this->asUint != other_.asUint;
	}

	inline const bool operator<(const TimeStamp& other_) const
	{
		return this->asFloat < other_.asFloat;
	}
	inline const bool operator>(const TimeStamp& other_) const
	{
		return this->asFloat > other_.asFloat;
	}
}; // union TimeStamp

} // namespace ecs
} // namespace raptor_engine