#include "ecs/i_entity.hpp"

using namespace raptor_engine::ecs;

i_entity::i_entity() 
	: is_active {true}
	, id {INVALID_ENTITY_ID}
	, component_manager_instance {new component_manager()} 
{

}

i_entity::i_entity(const entity_id& id_, component_manager* component_manager_) 
	: is_active {true}
	, id {id_}
	, component_manager_instance {component_manager_}
{

}

i_entity::~i_entity() = default;

void i_entity::set_is_active(bool is_active_)
{
	if (this->is_active == is_active_)
	{
		return;
	}

	if (is_active_)
	{
		this->on_enable();
	}
	else
	{
		this->on_disable();
	}
	this->is_active = is_active_;
}