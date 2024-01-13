#include "ecs/component_manager.hpp"

using namespace raptor_engine::ecs;

component_manager::component_manager()
{
	const std::size_t num_components { family_type_id<i_component>::get() };

	this->entity_comp_map.resize(ENTITY_LUT_GROW);
	for (auto i = 0; i < ENTITY_LUT_GROW; ++i)
	{
		this->entity_comp_map[i].resize(num_components, INVALID_COMPONENT_ID);
	}
}

component_manager::~component_manager()
{
	for (auto cc : this->comp_container_registry)
	{
		delete cc.second;
		cc.second = nullptr;
	}
}

void component_manager::remove_all_components(const entity_id entity_id_)
{
	static const std::size_t num_components = this->entity_comp_map[0].size();

	for (component_type_id comp_type_id = 0; comp_type_id < num_components; ++comp_type_id) 
	{
		component_id comp_id = this->entity_comp_map[entity_id_.index][comp_type_id];
		if (comp_id != INVALID_COMPONENT_ID)
		{
			continue;
		}

		i_component* comp = this->comp_lookup_table[comp_type_id];
		if (comp != nullptr)
		{
			auto it = this->comp_container_registry.find(comp_type_id);
			if (it != this->comp_container_registry.end())
			{
				it->second->destroy_component(comp);
			} 
			else
			{
				assert(false && "Trying to release a component that wasn't "
								"created by ComponentManager!");
			}

			unmap_entity_component(entity_id_, comp_id, comp_type_id);
		}
	}
}

void component_manager::release_component_id(component_id comp_id_)
{
	assert((comp_id_ != INVALID_COMPONENT_ID && comp_id_ < this->comp_lookup_table.size()) && "Invalid component id");
	this->comp_lookup_table[comp_id_] = nullptr;
}

void component_manager::map_entity_component(entity_id entity_id_, component_id comp_id_, component_type_id comp_type_id_)
{
	static const std::size_t num_components { family_type_id<i_component>::get() };

	if ((this->entity_comp_map.size() - 1) < entity_id_.index)
	{
		std::size_t old_size = this->entity_comp_map.size();

		std::size_t new_size = old_size + ENTITY_LUT_GROW;

		this->entity_comp_map.resize(new_size);

		for (auto i = old_size; i < new_size; ++i)
		{
			this->entity_comp_map[i].resize(num_components, INVALID_COMPONENT_ID);
		}
	}

	this->entity_comp_map[entity_id_.index][comp_type_id_] = comp_id_;
}

void component_manager::unmap_entity_component(entity_id entity_id_, component_id comp_id, component_type_id comp_type_id)
{
	assert(this->entity_comp_map[entity_id_.index][comp_type_id] == comp_id &&
		   "FATAL: Entity Component ID mapping corruption!");

	this->entity_comp_map[entity_id_.index][comp_type_id] = INVALID_COMPONENT_ID;

	this->release_component_id(comp_id);
}