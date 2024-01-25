#include "ecs/entity/i_entity.hpp"

using namespace raptor_engine::ecs;

IEntity::IEntity() 
	: isActive {true}
	, id {INVALID_ENTITY_ID}
	, componentManager {new ComponentManager()} {

}

IEntity::IEntity(const EntityId& id_, ComponentManager* componentManager_) 
	: isActive {true}
	, id {id_}
	, componentManager {componentManager_}
{

}

IEntity::~IEntity() = default;

void IEntity::SetIsActive(bool active_)
{
	if (this->isActive == active_)
	{
		return;
	}

	if (active_)
	{
		this->onEnable();
	}
	else
	{
		this->onDisable();
	}
	this->isActive = active_;
}