#include "ecs/entity_manager.hpp"

using namespace raptor_engine::ecs;

entity_manager::entity_manager(component_manager* component_manager_instance_) 
	: pending_destroyed_ent{1024}
	, num_pending_destroyed_ent {0}
	, component_manager_instance {component_manager_instance_}
{

}

entity_manager::~entity_manager()
{
	for (auto ec : this->ent_registry)
	{
		delete ec.second;
		ec.second = nullptr;
	}
}

entity_id entity_manager::acquire_entity_id(i_entity* entity_)
{
	return this->ent_handle_table.acquire_handle(entity_);
}

void entity_manager::release_entity_id(entity_id id_)
{
	this->ent_handle_table.release_handle(id_);
}

void entity_manager::remove_destroyed_entities()
{
	for (std::size_t i = 0; i < this->num_pending_destroyed_ent; ++i) 
	{ 
	    entity_id id = this->pending_destroyed_ent[i];

		i_entity* entity = this->ent_handle_table[id];

		const entity_type_id ETID = entity->get_static_entity_type_id();

		auto it = this->ent_registry.find(ETID);
		if (it != this->ent_registry.end())
		{
			this->component_manager_instance->remove_all_components(id);

			it->second->destroy_entity(entity);
		}

		this->release_entity_id(id);
	}

	this->num_pending_destroyed_ent = 0;
}

void entity_manager::destroy_entity(entity_id id_)
{
	if (this->num_pending_destroyed_ent < this->pending_destroyed_ent.size())
	{
		this->pending_destroyed_ent[this->num_pending_destroyed_ent++] = id_;
	}
	else
	{
		this->pending_destroyed_ent.push_back(id_);
		this->num_pending_destroyed_ent++;
	}
}