#pragma once

#include "ecs/icomponent.hpp"

namespace raptor_engine {
namespace ecs {

using component_type_id = type_id;

template<typename T>
class component : public icomponent
{
public:
	static const component_type_id STATIC_COMPONENT_TYPE_ID;

	component() = default;

	virtual ~component() = default;

	component_type_id get_static_component_type_id() const
	{
		return STATIC_COMPONENT_TYPE_ID;
	}
};

} // namespace ecs
} // namespace raptor_engine