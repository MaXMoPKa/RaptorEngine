#pragma once

#include "ecs/component/component_manager.hpp"

namespace raptor_engine {
namespace ecs {

	using EntityTypeId = TypeId;

class IEntity
{
	friend class entity_manager;

public:
	IEntity();
	IEntity(const EntityId& entityId_, ComponentManager* componentManager_);
	virtual ~IEntity();

public:
	template <typename T>
	T* GetComponent() const
	{
		return this->componentManager->GetComponent<T>(this->id);
	}

	template <typename T, class... P>
	T* AddComponent(P&&... params_)
	{
		return this->componentManager->AddComponent<T>(this->id, std::forward<P>(params_)...);
	}

	template <typename T>
	void remove_component()
	{
		this->componentManager->RemoveComponent<T>(this->id);
	}

public:
	virtual void onEnable() {}
	virtual void onDisable() {}

public:

	inline bool operator==(const IEntity& other_) const
	{
		return this->id == other_.id;
	}
	inline bool operator!=(const IEntity& other_) const
	{
		return this->id != other_.id;
	}
	inline bool operator==(const IEntity* other_) const
	{
		return this->id == other_->id;
	}
	inline bool operator!=(const IEntity* other_) const
	{
		return this->id != other_->id;
	}

public:
	virtual const EntityTypeId GetStaticEntityTypeId() const = 0;

	inline const EntityId getId() const
	{
		return this->id;
	}

	void SetIsActive(bool active_);

	inline bool GetIsActive() const
	{
		return this->isActive;
	}
	

protected:
	EntityId id;
	bool	 isActive;

private:
	ComponentManager* componentManager;
};

using IEntityUptr = std::unique_ptr<IEntity>;
using IEntitySptr = std::shared_ptr<IEntity>;
using IEntityWptr = std::weak_ptr<IEntity>;

} // namespace ecs
} // namespace raptor_engine