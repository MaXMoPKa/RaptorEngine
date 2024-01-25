#pragma once

#include "ecs/platform.hpp"
#include "ecs/handle.hpp"

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
	const ComponentId getHash() const;

	const ComponentId getId() const;

	const EntityId getOwnerId() const;

	bool getIsActive() const;

	void setHash(const ComponentId hash_);

	void setId(const ComponentId id_);

	void setOwnerId(const EntityId owner_id_);

	void setIsActive(bool is_active_);

protected:
	ComponentId	hashValue;
	ComponentId	id;
	EntityId	ownerId;
	bool		isActive;
};

} // namespace ecs
} // namespace raptor_engine