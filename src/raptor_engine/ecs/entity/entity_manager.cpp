#include "ecs/entity/entity_manager.hpp"

using namespace raptor_engine::ecs;

EntityManager::EntityManager(const ComponentManagerSptr& componentManager_) 
	: pendingDestroyedEntity{1024}
	, numPendingDestroyedEntity {0}
	, componentManager {componentManager_}
{

}

EntityManager::~EntityManager()
{
	for (auto entityContainer : this->entityRegistry)
	{
		delete entityContainer.second;
		entityContainer.second = nullptr;
	}
}

EntityId EntityManager::AcquireEntityId(IEntity* entity_)
{
	return this->entityHandleTable.AcquireHandle(entity_);
}

void EntityManager::ReleaseEntityId(EntityId id_)
{
	this->entityHandleTable.ReleaseHandle(id_);
}

void EntityManager::RemoveDestroyedEntities()
{
	for (std::size_t i = 0; i < this->numPendingDestroyedEntity; ++i) 
	{ 
	    EntityId id = this->pendingDestroyedEntity[i];

		IEntity* entity = this->entityHandleTable[id];

		const EntityTypeId entityTypeId = entity->GetStaticEntityTypeId();

		auto it = this->entityRegistry.find(entityTypeId);
		if (it != this->entityRegistry.end())
		{
			this->componentManager->RemoveAllComponents(id);

			it->second->DestroyEntity(entity);
		}

		this->ReleaseEntityId(id);
	}

	this->numPendingDestroyedEntity = 0;
}

void EntityManager::DestroyEntity(EntityId id_)
{
	if (this->numPendingDestroyedEntity < this->pendingDestroyedEntity.size())
	{
		this->pendingDestroyedEntity[this->numPendingDestroyedEntity++] = id_;
	}
	else
	{
		this->pendingDestroyedEntity.push_back(id_);
		this->numPendingDestroyedEntity++;
	}
}