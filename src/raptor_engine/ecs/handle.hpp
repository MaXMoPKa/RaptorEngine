#pragma once

#include <cstddef>
#include <climits>
#include <limits>

#include "defines.hpp"

namespace raptor_engine {
namespace ecs {
namespace util
{

template<typename handle_value_type, std::size_t version_bits, std::size_t index_bits>
union handle
{
	static_assert(sizeof(handle_value_type) * CHAR_BIT >= (version_bits + index_bits),
				 "Invalid handle layout. More bits used than base value type can hold!");

	using value_type = handle_value_type;

	static constexpr std::size_t NUM_VERSION_BITS { version_bits };
	static constexpr std::size_t NUM_INDEX_BITS { index_bits };

	static constexpr value_type MIN_VERSION { 0 };
	static constexpr value_type MAX_VERSION { (1U << NUM_VERSION_BITS) - 2U };
	static constexpr value_type MAX_INDICES { ((value_type)1U << NUM_INDEX_BITS) - 2U };

    static constexpr value_type INVALID_HANDLE { std::numeric_limits<value_type>::max() };

public:
	struct
	{
		value_type index : NUM_INDEX_BITS;
		value_type version : NUM_VERSION_BITS;
	};

	handle() = default;

	handle(value_type value_) 
		: value(value_)
	{

	}

	handle(value_type index_, value_type version_) 
		: index(index_)
		, version(version_)
	{

	}

	inline operator value_type() const
	{
		return value;
	}

private:
	value_type value;

};

using handle32 = handle<u32, 12, 20>;
using handle64 = handle<u64, 24, 40>;

} // namespace util
} // namespace ecs
} // namespace raptor_engine