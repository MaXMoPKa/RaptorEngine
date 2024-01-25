#pragma once

#include "ecs/component/i_component.hpp"

namespace raptor_engine {
namespace ecs {

using ComponentTypeId = TypeId;

template<typename T>
class Component : public IComponent
{
public:
	static const ComponentTypeId STATIC_COMPONENT_TYPE_ID;

	Component() = default;

	virtual ~Component() = default;

	ComponentTypeId GetStaticComponentTypeId() const
	{
		return STATIC_COMPONENT_TYPE_ID;
	}
};

} // namespace ecs
} // namespace raptor_engine