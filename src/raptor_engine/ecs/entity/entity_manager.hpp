#pragma once

#include "ecs/component/component_manager.hpp"
#include "ecs/entity/i_entity.hpp"
#include "ecs/handle_table.hpp"

namespace raptor_engine {
namespace ecs {

using EntityHandleTable = util::HandleTable<IEntity, EntityId>;

class EntityManager
{
    class IEntityContainer
    {
	public:

		virtual ~IEntityContainer() = default;
		
		virtual const char* GetEntityContainerTypeName() const = 0;

		virtual void DestroyEntity(IEntity* entity_) = 0;
    }; // IEntity_container

	template <typename T>
	class EntityContainer : public memory::allocator::memory_chunk_allocator<T, ENTITY_T_CHUNK_SIZE>,
							public IEntityContainer
	{
	public:
		EntityContainer() 
			: memory::allocator::memory_chunk_allocator<T, ENTITY_T_CHUNK_SIZE>("EntityManager")
		{ }

		virtual ~EntityContainer() { }

	private:
		EntityContainer(const EntityContainer&)      = delete;
		EntityContainer& operator=(EntityContainer&) = delete;

	public:
		virtual const char* GetEntityContainerTypeName() const override
		{
			static const char* ENTITY_TYPE_NAME {typeid(T).name()};
			return ENTITY_TYPE_NAME;
		}

		virtual void DestroyEntity(IEntity* enity_)
		{
			enity_->~IEntity();
			this->DestroyObject(enity_);
		}
	}; // entity_container

public:
	EntityManager(ComponentManager* componentManager_);
	~EntityManager();

private:
	EntityManager(const EntityManager&)		  = delete;
	EntityManager& operator=(EntityManager&) = delete;

public:
	template <typename T, class... ARGS>
	EntityId CreateEntity(ARGS&&... args_)
	{
		void* objectMemoryPointer = GetEntityContainer<T>()->CreateObject();

		EntityId newEntityId = this->AcquireEntityId((T*)objectMemoryPointer);

		IEntity* entity =
			new (objectMemoryPointer) T(newEntityId, this->componentManager, std::forward<ARGS>(args_)...);

		return newEntityId;
	}

	void DestroyEntity(EntityId entityId_);

	inline IEntity* GetEntity(EntityId id_)
	{
		return this->entityHandleTable[id_];
	}

	inline EntityId GetEntityId(EntityId::value_type index_) const
	{
		return this->entityHandleTable[index_];
	}

	void RemoveDestroyedEntities();

private:
	template <typename T>
	inline EntityContainer<T>* GetEntityContainer()
	{
		EntityTypeId entityTypeId = T::STATIC_ENTITY_TYPE_ID;

		auto it = this->EntityRegistry.find(entityTypeId);
		EntityContainer<T>* container = nullptr;

		if (it == this->EntityRegistry.end())
		{
			container = new EntityContainer<T>();
			this->EntityRegistry[entityTypeId] = container;
		}
		else
		{
			container = (EntityContainer<T>*)it->second;
		}

		assert(container != nullptr && "Failed to create entity_container<T>!");
		return container;
	}

	EntityId AcquireEntityId(IEntity* entity_);

	void ReleaseEntityId(EntityId id_);

private:
	using EntityRegistry           = std::unordered_map<EntityTypeId, IEntityContainer*>;
	using PendingDestroyedEntities = std::vector<EntityId>;
	EntityRegistry           entityRegistry;
	PendingDestroyedEntities pendingDestroyedEntity;
	std::size_t				 numPendingDestroyedEntity;
	ComponentManager*		 componentManager;
	EntityHandleTable		 entityHandleTable;
};

} // namespace ecs
} // namespace raptor_engine