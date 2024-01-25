#include "ecs/component/i_component.hpp"

using namespace raptor_engine::ecs;

IComponent::IComponent() 
	: hashValue {0}
	, id {0}
	, ownerId {INVALID_ENTITY_ID}
	, isActive {true}
{

}

IComponent::~IComponent() = default;

bool IComponent::operator==(const IComponent& other_) const
{
	return this->hashValue == other_.hashValue;
}

bool IComponent::operator!=(const IComponent& other_) const
{
	return this->hashValue != other_.hashValue;
}

const ComponentId IComponent::GetHash() const
{
	return this->hashValue;
}

const ComponentId IComponent::GetId() const
{
	return this->id;
}

const EntityId IComponent::GetOwnerId() const
{
	return this->ownerId;
}

bool IComponent::GetIsActive() const
{
	return this->isActive;
}

void IComponent::SetHash(const ComponentId hash_)
{
	this->hashValue = hash_;
}

void IComponent::SetId(const ComponentId id_)
{
	this->id = id_;
}

void IComponent::SetOwnerId(const EntityId ownerId_)
{
	this->ownerId = ownerId_;
}

void IComponent::SetIsActive(bool state_)
{
	this->isActive = state_;
}