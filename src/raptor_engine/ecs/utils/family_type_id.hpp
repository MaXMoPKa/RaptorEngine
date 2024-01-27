#pragma once

#include "ecs/utils/platform.hpp"

namespace raptor_engine 
{
namespace ecs 
{
namespace util
{

template<class T>
class FamilyTypeId
{
public:

	template<class U>
	static const TypeId get()
	{
		static const TypeId STATIC_TYPE_ID {count++};
		return STATIC_TYPE_ID;
	}

	static TypeId get()
	{
		return count;
	}

private:

	inline static TypeId count = 0;
};

} // namespace util
} // namespace ecs
} // namespace raptor_engine