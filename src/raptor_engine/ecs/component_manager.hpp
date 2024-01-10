#pragma once

#include <functional>

#include "ecs/component.hpp"
#include "ecs/family_type_id.hpp"

#include "memory/global_memory_user.hpp"

namespace raptor_engine {
namespace ecs {

static const component_id INVALID_COMPONENT_ID = INVALID_OBJECT_ID;

template <typename T>
const component_type_id component<T>::STATIC_COMPONENT_TYPE_ID = family_type_id<i_component>::get<T>();

class component_manager : memory::global_memory_user
{
	friend class i_component;

	class i_component_container
	{
	public:

		virtual ~i_component_container() = default;

		virtual const char* get_component_container_type_name() const = 0;

		virtual void destroy_component(i_component* object) = 0;
	};

	template<typename T>
	class component_container : public memory::memory_chunk_allocator<T, COMPONENT_T_CHUNK_SIZE>,
								public i_component_container
	{
		component_container(const component_container&)			   = delete;
		component_container& operator=(const component_container&) = delete;

	public:

		component_container() : memory::memory_chunk_allocator<T, COMPONENT_T_CHUNK_SIZE>("component_manager") { }

		virtual ~component_container() = default;

		virtual const char* get_component_container_type_name() const override
		{
			static const char* COMPONENT_TYPE_NAME {typeid(T).name()};
			return COMPONENT_TYPE_NAME;
		}

		virtual void destroy_component(i_component* object_) override
		{
			object_->~i_component();
			this->destroy_object(object_);
		}
	};

public:

	template<typename T>
	using t_component_iterator = typename component_container<T>::iterator;

	component_manager();
	~component_manager();

	component_manager(const component_manager&)			   = delete;
	component_manager& operator=(const component_manager&) = delete;

public:

	template<typename T, class... ARGS>
	T* add_component(const entity_id entity_id_, ARGS&&... args_)
	{
		static constexptr std::hash<component_id> entity_component_id_hasher {std::hash<component_id>()};

		const component_type_id CTID = T::STATIC_COMPONENT_TYPE_ID;

		void* p_object_memory = get_component_container<T>()->create_object();

		component_id comp_id		   = this->acquire_component_id((T*)p_object_memeory);
		((T*)p_object_memory)->comp_id = comp_id;

		i_component* comp = new (p_object_memory) T(std::forward<ARGS>(args_)...);

		comp->set_owner_id(entity_id_);
		component_id hash = entity_component_id_hasher(entity_id_) ^ (entity_component_id_hasher(comp_id) << 1);
		comp->set_hash(hash);

		map_entity_component(entity_id_, comp_id, CTID);

		return static_cast<T*>(component);
	}

	template<typename T>
	void remove_component(const entity_id entity_id_)
	{
		const component_type_id comp_type_id = T::STATIC_COMPONENT_TYPE_ID;

		const component_id comp_id = this->entity_comp_map[entity_id_.index][comp_type_id];

		i_component* comp = this->comp_lookup_table[comp_id];

		assert(comp != nullptr && "FATAL: Trying to remove a component "
								  "watch is not used by this entity!");

		get_component_container<T>()->destroy_object(comp);

		unmap_entity_component(entity_id_, comp_id, comp_type_id);
	}

	void remove_all_components(const entity_id entity_id_);

	template <typename T>
	T* get_component(const entity_id entity_id_)
	{
		const component_type_id comp_type_id = T::STATIC_COMPONENT_TYPE_ID;

		const component_id comp_id = this->entity_comp_map[entity_id_.index][comp_type_id];

		if (comp_id == INVALID_COMPONENT_ID)
		{
			return nullptr;
		}

		return static_cast<T*>(this->comp_lookup_table[comp_id]);
	}

	template <typename T>
	inline t_component_iterator<T> begin()
	{
		return get_component_container<T>()->begin();
	}

	template <typename T>
	inline t_component_iterator<T> end()
	{
		return get_component_container<T>()->end();
	}

private:
    template <typename T>
	inline component_container<T>* get_component_container()
	{
		component_type_id comp_type_id = T::STATIC_COMPONENT_TYPE_ID;

		auto					it = this->comp_container_registry.find(comp_type_id);
		component_container<T>* cc = nullptr;

		if (it == this->comp_container_registry.end())
		{
			cc                                               = new component_container<T>();
			this->comp_container_registry[comp_type_id] = cc;
		}
		else
		{
			cc = static_cast<component_container<T>*>(it->second);
		}

		assert(cc != nullptr && "ERROR: Failed to create component_container<T>!");
		return cc;
	}

	component_id acquire_component_id(i_component* comp_)
	{
		int i = 0;
		for (; i < this->comp_lookup_table.size(); ++i)
		{
			if (this->comp_lookup_table[i] == nullptr)
			{
				this->comp_lookup_table[i] = comp_;
				return i;
			}
		}

		this->comp_lookup_table.resize(this->comp_lookup_table.size() + COMPONENT_LUT_GROW, nullptr);
		this->comp_lookup_table[i] = comp_;
		return i;
	}

	void release_component_id(component_id comp_id_);

	void map_entity_component(entity_id entity_id_, component_id comp_id, component_type_id comp_type_id);

	void unmap_entity_component(entity_id entity_id_, component_id comp_id, component_type_id comp_type_id);

private:
	using component_container_registry = std::unordered_map<component_type_id, i_component_container*>;
	component_container_registry comp_container_registry;

	using component_lookup_table = std::vector<i_component*>;
	component_lookup_table comp_lookup_table;

	using entity_component_map = std::vector<std::vector<component_id>>;
	entity_component_map entity_comp_map;
};
} // namespace ecs
} // namespace raptor_engine