#pragma once

#include <cstddef>
#include <climits>
#include <limits>

#include "defines.hpp"

namespace raptor_engine {
namespace ecs {
namespace util
{

template<typename HandleValueType, std::size_t VersionBits, std::size_t IndexBits>
union Handle
{
	static_assert(sizeof(HandleValueType) * CHAR_BIT >= (VersionBits + IndexBits),
				  "Invalid handle layout. More bits used than base value type can hold!");

	using value_type = HandleValueType;

	static constexpr std::size_t NUM_VERSION_BITS {VersionBits};
	static constexpr std::size_t NUM_INDEX_BITS {IndexBits};

	static constexpr value_type MIN_VERSION {0};
	static constexpr value_type MAX_VERSION {(1U << NUM_VERSION_BITS) - 2U};
	static constexpr value_type MAX_INDICES {((value_type)1U << NUM_INDEX_BITS) - 2U};

	static constexpr value_type INVALID_HANDLE {std::numeric_limits<value_type>::max()};

public:

	struct
	{
		value_type index : NUM_INDEX_BITS;
		value_type version : NUM_VERSION_BITS;
	};

	Handle() = default;

	Handle(value_type value_) : value(value_) { }

	Handle(value_type index_, value_type version_) 
		: index{index_}
		, version{version_}
	{

	}

	inline operator value_type() const
	{
		return value;
	}

private:
	value_type value;

};

using Handle32 = Handle<u32, 12, 20>;
using Handle64 = Handle<u64, 24, 40>;

} // namespace util
} // namespace ecs
} // namespace raptor_engine