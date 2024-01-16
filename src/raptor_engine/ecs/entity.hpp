#pragma once

#include "ecs/i_entity.hpp"

namespace raptor_engine {
namespace ecs {

template<typename T>
class entity : public i_entity
{
public:
	static const entity_type_id STATIC_ENTITY_TYPE_ID;

public:
	entity() = default;
	entity(const entity_id& id_, component_manager* component_manager_) 
		: i_entity {id_, component_manager_}
	{

	}

	virtual ~entity() = default;

public:
	entity_type_id get_static_entity_type_id() const
	{
		return STATIC_ENTITY_TYPE_ID;
	}
};

} // namespace ecs
} // namespace raptor_engine