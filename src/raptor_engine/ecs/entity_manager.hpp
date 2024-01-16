#pragma once

#include "ecs/component_manager.hpp"
#include "ecs/i_entity.hpp"
#include "ecs/handle_table.hpp"

namespace raptor_engine {
namespace ecs {

using entity_handle_table = util::handle_table<i_entity, entity_id>;

class entity_manager
{
    class i_entity_container
    {
	public:

		virtual ~i_entity_container() = default;
		
		virtual const char* get_entity_container_type_name() const = 0;

		virtual void destroy_entity(i_entity* object_) = 0;
    }; // i_entity_container

	template <typename T>
	class entity_container : public memory::allocator::memory_chunk_allocator<T, ENTITY_T_CHUNK_SIZE>,
							 public i_entity_container
	{
	public:

		entity_container() 
			: memory::allocator::memory_chunk_allocator<T, ENTITY_T_CHUNK_SIZE>("entity_manager")
		{ }

		virtual ~entity_container() { }

	private:
		entity_container(const entity_container&)      = delete;
		entity_container& operator=(entity_container&) = delete;

	public:
		virtual const char* get_entity_container_type_name() const override
		{
			static const char* ENTITY_TYPE_NAME {typeid(T).name()};
			return ENTITY_TYPE_NAME;
		}

		virtual void destroy_entity(i_entity* object_)
		{
			object_->~i_entity();
			this->destroy_object(object_);
		}
	}; // entity_container

public:
	entity_manager(component_manager* component_manager_instance_);
	~entity_manager();

private:
	entity_manager(const entity_manager&)      = delete;
	entity_manager& operator=(entity_manager&) = delete;

public:
	template <typename T, class... ARGS>
	entity_id create_entity(ARGS&&... args)
	{
		void* p_object_memory = get_entity_container<T>()->create_object();

		entity_id new_entity_id = this->acquire_entity_id((T*)p_object_memory);

		i_entity* entity =
			new (p_object_memory) T(new_entity_id, this->component_manager_instance, std::forward<ARGS>(args)...);

		return new_entity_id;
	}

	void destroy_entity(entity_id entity_id_);

	inline i_entity* get_entity(entity_id id_)
	{
		return this->ent_handle_table[id_];
	}

	inline entity_id get_entity_id(entity_id::value_type index) const
	{
		return this->ent_handle_table[index];
	}

	void remove_destroyed_entities();

private:
	template <typename T>
	inline entity_container<T>* get_entity_container()
	{
		entity_type_id EID = T::STATIC_ENTITY_TYPE_ID;

		auto                 it = this->entity_registry.find(EID);
		entity_container<T>* ec = nullptr;

		if (it == this->entity_registry.end())
		{
			ec                         = new entity_container<T>();
			this->entity_registry[EID] = ec;
		}
		else
		{
			ec = (entity_container<T>*)it->second;
		}

		assert(ec != nullptr && "Failed to create entity_container<T>!");
		return ec;
	}

	entity_id acquire_entity_id(i_entity* entity_);

	void release_entity_id(entity_id id_);

private:
	using entity_registry            = std::unordered_map<entity_type_id, i_entity_container*>;
	using pending_destroyed_entities = std::vector<entity_id>;
	entity_registry            ent_registry;
	pending_destroyed_entities pending_destroyed_ent;
	std::size_t				   num_pending_destroyed_ent;
	component_manager*		   component_manager_instance;
	entity_handle_table		   ent_handle_table;
};

} // namespace ecs
} // namespace raptor_engine