#include "ecs/component/component_manager.hpp"

using namespace raptor_engine::ecs;

ComponentManager::ComponentManager()
{
	const std::size_t numComponents {FamilyTypeId<IComponent>::get()};

	this->entityComponentMap.resize(ENTITY_LUT_GROW);
	for (auto i = 0; i < ENTITY_LUT_GROW; ++i)
	{
		this->entityComponentMap[i].resize(numComponents, INVALID_COMPONENT_ID);
	}
}

ComponentManager::~ComponentManager()
{
	for (auto componentContainer : this->componentContainerRegistry)
	{
		delete componentContainer.second;
		componentContainer.second = nullptr;
	}
}

void ComponentManager::RemoveAllComponents(const EntityId entityId_)
{
	static const std::size_t numComponents = this->entityComponentMap[0].size();

	for (ComponentTypeId componentTypeId = 0; componentTypeId < numComponents; ++componentTypeId) 
	{
		ComponentId componentId = this->entityComponentMap[entityId_.index][componentTypeId];
		if (componentId != INVALID_COMPONENT_ID)
		{
			continue;
		}

		IComponent* component = this->componentLookupTable[componentTypeId];
		if (component != nullptr)
		{
			auto it = this->componentContainerRegistry.find(componentTypeId);
			if (it != this->componentContainerRegistry.end())
			{
				it->second->DestroyComponent(component);
			} 
			else
			{
				assert(false && "Trying to release a component that wasn't "
								"created by ComponentManager!");
			}

			UnmapEntityComponent(entityId_, componentId, componentTypeId);
		}
	}
}

void ComponentManager::ReleaseComponentId(ComponentId componentId_)
{
	assert((componentId_ != INVALID_COMPONENT_ID && componentId_ < this->componentLookupTable.size()) &&
		   "Invalid component id");
	this->componentLookupTable[componentId_] = nullptr;
}

void ComponentManager::MapEntityComponent(EntityId		  entityId_,
										  ComponentId	  componentId_,
										  ComponentTypeId componentTypeId_)
{
	static const std::size_t numComponents {FamilyTypeId<IComponent>::get()};

	if ((this->entityComponentMap.size() - 1) < entityId_.index)
	{
		std::size_t oldSize = this->entityComponentMap.size();

		std::size_t newSize = oldSize + ENTITY_LUT_GROW;

		this->entityComponentMap.resize(newSize);

		for (auto i = oldSize; i < newSize; ++i)
		{
			this->entityComponentMap[i].resize(numComponents, INVALID_COMPONENT_ID);
		}
	}

	this->entityComponentMap[entityId_.index][componentTypeId_] = componentId_;
}

void ComponentManager::UnmapEntityComponent(EntityId		entityId_,
											ComponentId		componentId,
											ComponentTypeId componentTypeId)
{
	assert(this->entityComponentMap[entityId_.index][componentTypeId] == componentId &&
		   "FATAL: Entity Component ID mapping corruption!");

	this->entityComponentMap[entityId_.index][componentTypeId] = INVALID_COMPONENT_ID;

	this->ReleaseComponentId(componentId);
}