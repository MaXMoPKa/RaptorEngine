#include "ecs/icomponent.hpp"

using namespace raptor_engine::ecs;

i_component::i_component() 
	: hash_value {0}
	, id {0}
	, owner_id {INVALID_ENTITY_ID}
	, is_active {true}
{

}

i_component::~i_component() = default;

bool i_component::operator==(const i_component& other_) const
{
	return this->hash_value == other_.hash_value;
}

bool i_component::operator!=(const i_component& other_) const
{
	return this->hash_value != other_.hash_value;
}

const component_id i_component::get_hash() const
{
	return this->hash_value;
}

const component_id i_component::get_id() const
{
	return this->id;
}

const entity_id i_component::get_owner_id() const
{
	return this->owner_id;
}

bool i_component::get_is_active() const
{
	return this->is_active;
}

void i_component::set_hash(const component_id hash_)
{
	this->hash_value = hash_;
}

void i_component::set_id(const component_id id_)
{
	this->id = id_;
}

void i_component::set_owner_id(const entity_id owner_id_)
{
	this->owner_id = owner_id_;
}

void i_component::set_is_active(bool state_)
{
	this->is_active = state_;
}