#pragma once

#include "ecs/component_manager.hpp"

namespace raptor_engine {
namespace ecs {

	using entity_type_id = type_id;

class i_entity
{
	friend class entity_manager;

public:
	i_entity();
	i_entity(const ecs::entity_id& entity_id_, ecs::component_manager* component_manager_);
	virtual ~i_entity();

public:
	template <typename T>
	T* get_component() const
	{
		return this->component_manager_instance->get_component<T>(this->id);
	}

	template <typename T, class... P>
	T* add_component(P&&... params)
	{
		return this->component_manager_instance->add_component<T>(this->id);
	}

	template <typename T>
	void remove_component()
	{
		this->component_manager_instance->remove_component<T>(this->id);
	}

public:
	virtual void on_enable() {}
	virtual void on_disable() {}

public:

	inline bool operator==(const i_entity& rhs) const
	{
		return this->id == rhs.id;
	}
	inline bool operator!=(const i_entity& rhs) const
	{
		return this->id != rhs.id;
	}
	inline bool operator==(const i_entity* rhs) const
	{
		return this->id == rhs->id;
	}
	inline bool operator!=(const i_entity* rhs) const
	{
		return this->id != rhs->id;
	}

public:
	virtual const entity_type_id get_static_entity_type_id() const = 0;

	inline const entity_id get_id() const
	{
		return this->id;
	}

	void set_is_active(bool active);

	inline bool get_is_active() const
	{
		return this->is_active;
	}
	

protected:
	entity_id id;
	bool	  is_active;

private:
	component_manager* component_manager_instance;
};

} // namespace ecs
} // namespace raptor_engine