#pragma once

#include "ecs/i_system.hpp"
#include "ecs/family_type_id.hpp"

namespace raptor_engine {
namespace ecs {

template <typename T>
class system : public i_system
{
	friend class system_manager;

public:
	static const system_type_id STATIC_SYSTEM__TYPE_ID;

public:
	virtual ~system() = default;

public:
	virtual inline const system_type_id get_static_system_type_id() const
	{
		return STATIC_SYSTEM__TYPE_ID;
	}

	virtual inline const char* get_system_type_name() const override
	{
		static const char* SYSTEM_TYPE_NAME {typeid(T).name()};
		return SYSTEM_TYPE_NAME;
	}

	template <typename... Dependencies>
	void add_dependencies(Dependencies&&... dependencies)
	{
		this->system_manager->add_system_dependency(this, std::forward<Dependencies>(dependencies)...);
	}

	virtual void pre_update(f32 dt) override { }

	virtual void update(f32 dt) override { }

	virtual void post_update(f32 dt) override { }

private:
	system_manager* manager;

};

template <typename T>
const system_type_id system<T>::STATIC_SYSTEM__TYPE_ID = family_type_id<i_system>::get<T>();


} // namespace ecs
} // namespace raptor_engine