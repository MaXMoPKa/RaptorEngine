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

const ComponentId IComponent::getHash() const
{
	return this->hashValue;
}

const ComponentId IComponent::getId() const
{
	return this->id;
}

const EntityId IComponent::getOwnerId() const
{
	return this->ownerId;
}

bool IComponent::getIsActive() const
{
	return this->isActive;
}

void IComponent::setHash(const ComponentId hash_)
{
	this->hashValue = hash_;
}

void IComponent::setId(const ComponentId id_)
{
	this->id = id_;
}

void IComponent::setOwnerId(const EntityId ownerId_)
{
	this->ownerId = ownerId_;
}

void IComponent::setIsActive(bool state_)
{
	this->isActive = state_;
}