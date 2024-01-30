#pragma once

#include "ecs/utils/platform.hpp"
#include "ecs/utils/handle.hpp"

namespace raptor_engine {
namespace ecs {

using EntityId = util::Handle64;

static const EntityId INVALID_ENTITY_ID = util::Handle64::INVALID_HANDLE;

using ComponentId = ObjectId;

static const ComponentId INVALID_COMPONENT_ID = INVALID_OBJECT_ID;

class IComponent
{
public:
	IComponent();

	virtual ~IComponent();

public:
	bool operator==(const IComponent& other_) const;
	bool operator!=(const IComponent& other_) const;

public:
	const ComponentId GetHash() const;

	const ComponentId GetId() const;

	const EntityId GetOwnerId() const;

	bool GetIsActive() const;

	void SetHash(const ComponentId hash_);

	void SetId(const ComponentId id_);

	void SetOwnerId(const EntityId owner_id_);

	void SetIsActive(bool is_active_);

protected:
	ComponentId	hashValue;
	ComponentId	id;
	EntityId	ownerId;
	bool		isActive;
};

using IComponentUptr = std::unique_ptr<IComponent>;
using IComponentSptr = std::shared_ptr<IComponent>;
using IComponentWptr = std::weak_ptr<IComponent>;

} // namespace ecs
} // namespace raptor_engine