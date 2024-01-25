#pragma once

#include <functional>

#include "ecs/component/component.hpp"
#include "ecs/family_type_id.hpp"

#include "memory/global_memory_user.hpp"
#include "memory/allocators/memory_chunk_allocator.hpp"

namespace raptor_engine {
namespace ecs {

template <typename T>
const ComponentTypeId Component<T>::STATIC_COMPONENT_TYPE_ID = family_type_id<IComponent>::get<T>();

class ComponentManager : memory::global_memory_user
{
	friend class IComponent;

	class IComponentContainer
	{
	public:

		virtual ~IComponentContainer() = default;

		virtual const char* GetComponentContainerTypeName() const = 0;

		virtual void DestroyComponent(IComponent* object) = 0;
	};

	template<typename T>
	class ComponentContainer : public memory::allocator::memory_chunk_allocator<T, COMPONENT_T_CHUNK_SIZE>,
							   public IComponentContainer
	{
		ComponentContainer(const ComponentContainer&)			 = delete;
		ComponentContainer& operator=(const ComponentContainer&) = delete;

	public:

		ComponentContainer() : memory::allocator::memory_chunk_allocator<T, COMPONENT_T_CHUNK_SIZE>("component_manager")
		{ }

		virtual ~ComponentContainer() = default;

		virtual const char* GetComponentContainerTypeName() const override
		{
			static const char* COMPONENT_TYPE_NAME {typeid(T).name()};
			return COMPONENT_TYPE_NAME;
		}

		virtual void DestroyComponent(IComponent* component_) override
		{
			component_->~IComponent();
			this->DestroyObject(component_);
		}
	};

public:

	template<typename T>
	using TComponentIterator = typename ComponentContainer<T>::iterator;

	ComponentManager();
	~ComponentManager();

	ComponentManager(const ComponentManager&)			 = delete;
	ComponentManager& operator=(const ComponentManager&) = delete;

public:

	template<typename T, class... ARGS>
	T* AddComponent(const EntityId entityId_, ARGS&&... args_)
	{
		static constexpr std::hash<ComponentId> EntityComponentIdHasher {std::hash<ComponentId>()};

		const ComponentTypeId componentTypeId = T::STATIC_COMPONENT_TYPE_ID;

		void* objectMemoryPointer = GetComponentContainer<T>()->CreateObject();

		ComponentId componentId				   = this->acquire_component_id((T*)objectMemoryPointer);
		((T*)objectMemoryPointer)->componentId = componentId;

		IComponent* component = new (objectMemoryPointer) T(std::forward<ARGS>(args_)...);

		component->SetOwnerId(entityId_);
		ComponentId hash = EntityComponentIdHasher(entityId_) ^ (EntityComponentIdHasher(componentId) << 1);
		component->SetHash(hash);

		mapEntityComponent(entityId_, componentId, componentTypeId);

		return static_cast<T*>(component);
	}

	template<typename T>
	void RemoveComponent(const EntityId entityId_)
	{
		const ComponentTypeId componentTypeId = T::STATIC_COMPONENT_TYPE_ID;

		const ComponentId componentId = this->entityComponentMap[entityId_.index][componentTypeId];

		IComponent* component = this->componentLookupTable[componentId];

		assert(component != nullptr && "FATAL: Trying to remove a component "
								       "watch is not used by this entity!");

		GetComponentContainer<T>()->DestroyObject(component);

		UnmapEntityComponent(entityId_, componentId, componentTypeId);
	}

	void RemoveAllComponents(const EntityId entityId_);

	template <typename T>
	T* GetComponent(const EntityId entityId_)
	{
		const ComponentTypeId componentTypeId = T::STATIC_COMPONENT_TYPE_ID;

		const ComponentId componentId = this->entityComponentMap[entityId_.index][componentTypeId];

		if (componentId == INVALID_COMPONENT_ID)
		{
			return nullptr;
		}

		return static_cast<T*>(this->componentLookupTable[componentId]);
	}

	template <typename T>
	inline TComponentIterator<T> begin()
	{
		return GetComponentContainer<T>()->begin();
	}

	template <typename T>
	inline TComponentIterator<T> end()
	{
		return GetComponentContainer<T>()->end();
	}

private:
    template <typename T>
	inline ComponentContainer<T>* GetComponentContainer()
	{
		ComponentTypeId componentTypeId = T::STATIC_COMPONENT_TYPE_ID;

		auto it = this->componentContainerRegistry.find(componentTypeId);
		ComponentContainer<T>* componentContainer = nullptr;

		if (it == this->componentContainerRegistry.end())
		{
			componentContainer = new ComponentContainer<T>();
			this->componentContainerRegistry[componentTypeId] = componentContainer;
		}
		else
		{
			componentContainer = static_cast<ComponentContainer<T>*>(it->second);
		}

		assert(componentContainer != nullptr && "ERROR: Failed to create component_container<T>!");
		return componentContainer;
	}

	ComponentId AcquireComponentId(IComponent* component_)
	{
		int i = 0;
		for (; i < this->componentLookupTable.size(); ++i)
		{
			if (this->componentLookupTable[i] == nullptr)
			{
				this->componentLookupTable[i] = component_;
				return i;
			}
		}

		this->componentLookupTable.resize(this->componentLookupTable.size() + COMPONENT_LUT_GROW, nullptr);
		this->componentLookupTable[i] = component_;
		return i;
	}

	void ReleaseComponentId(ComponentId componentId_);

	void MapEntityComponent(EntityId entityId_, ComponentId componentId_, ComponentTypeId componentTypeId_);

	void UnmapEntityComponent(EntityId entityId_, ComponentId componentId_, ComponentTypeId componentTypeId_);

private:
	using ComponentContainerRegistry = std::unordered_map<ComponentTypeId, IComponentContainer*>;
	ComponentContainerRegistry componentContainerRegistry;

	using ComponentLookupTable = std::vector<IComponent*>;
	ComponentLookupTable componentLookupTable;

	using EntityComponentMap = std::vector<std::vector<ComponentId>>;
	EntityComponentMap entityComponentMap;
};

} // namespace ecs
} // namespace raptor_engine