#include "ecs/icomponent.hpp"

using namespace raptor_engine::ecs;

static const entity_id INVALID_ENTITY_ID = util::handle64::INVALID_HANDLE;

static const component_id INVALID_COMPONENT_ID = INVALID_OBJECT_ID;

icomponent::icomponent() 
	: hash_value {0}
	, id {0}
	, owner_id {INVALID_ENTITY_ID}
	, is_active {true}
{

}

icomponent::~icomponent() = default;

bool icomponent::operator==(const icomponent& other_) const
{
	return this->hash_value == other_.hash_value;
}

bool icomponent::operator!=(const icomponent& other_) const
{
	return this->hash_value != other_.hash_value;
}

const component_id icomponent::get_id() const
{
	return this->id;
}

const entity_id icomponent::get_owner_id() const
{
	return this->owner_id;
}

void icomponent::set_is_active(bool state_)
{
	this->is_active = state_;
}

bool icomponent::get_is_active() const
{
	return this->is_active;
}