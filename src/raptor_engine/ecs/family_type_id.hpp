#pragma once

#include "ecs/platform.hpp"

namespace raptor_engine {
namespace ecs {

template <class T>
class family_type_id
{
public:
	template <class U>
	static const type_id get()
	{
		static const type_id STATIC_TYPE_ID { s_count++ };
		return STATIC_TYPE_ID;
	}

	static type_id get()
	{
		return s_count;
	}

private:
	inline static type_id s_count = 0;
};

} // namespace ecs
} // namespace raptor_engine