#pragma once

#include "ecs/entity/i_entity.hpp"

namespace raptor_engine {
namespace ecs {

template<typename T>
class Entity : public IEntity
{
public:
	static const EntityTypeId STATIC_ENTITY_TYPE_ID;

public:
	Entity() = default;
	Entity(const EntityId& id_, ComponentManager* componentManager_) 
		: IEntity {id_, componentManager_}
	{

	}

	virtual ~Entity() = default;

public:
	EntityTypeId GetStaticEntityTypeId() const
	{
		return STATIC_ENTITY_TYPE_ID;
	}
};

} // namespace ecs
} // namespace raptor_engine