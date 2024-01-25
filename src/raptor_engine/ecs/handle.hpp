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

	using ValueType = HandleValueType;

	static constexpr std::size_t NUM_VERSION_BITS {VersionBits};
	static constexpr std::size_t NUM_INDEX_BITS {IndexBits};

	static constexpr ValueType	MIN_VERSION {0};
	static constexpr ValueType	MAX_VERSION {(1U << NUM_VERSION_BITS) - 2U};
	static constexpr ValueType	MAX_INDICES {((ValueType)1U << NUM_INDEX_BITS) - 2U};

    static constexpr ValueType INVALID_HANDLE {std::numeric_limits<ValueType>::max()};

public:
	struct
	{
		ValueType index   : NUM_INDEX_BITS;
		ValueType version : NUM_VERSION_BITS;
	};

	Handle() = default;

	Handle(ValueType value_) 
		: value(value_)
	{

	}

	Handle(ValueType index_, ValueType version_) 
		: index(index_)
		, version(version_)
	{

	}

	inline operator ValueType() const
	{
		return value;
	}

private:
	ValueType value;

};

using Handle32 = Handle<u32, 12, 20>;
using Handle64 = Handle<u64, 24, 40>;

} // namespace util
} // namespace ecs
} // namespace raptor_engine