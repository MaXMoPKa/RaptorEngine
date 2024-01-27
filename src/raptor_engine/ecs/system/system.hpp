#pragma once

#include "ecs/system/i_system.hpp"
#include "ecs/utils/family_type_id.hpp"

namespace raptor_engine {
namespace ecs {

template <typename T>
class System : public ISystem
{
	friend class SystemManager;

public:
	static const SystemTypeId STATIC_SYSTEM__TYPE_ID;

public:
	virtual ~System() = default;

public:
	virtual inline const SystemTypeId GetStaticSystemTypeId() const
	{
		return STATIC_SYSTEM__TYPE_ID;
	}

	virtual inline const char* GetSystemTypeName() const override
	{
		static const char* SYSTEM_TYPE_NAME {typeid(T).name()};
		return SYSTEM_TYPE_NAME;
	}

	template <typename... Dependencies>
	void AddDependencies(Dependencies&&... dependencies_)
	{
		this->SystemManager->AddSystemDependency(this, std::forward<Dependencies>(dependencies_)...);
	}

	virtual void PreUpdate(f32 dt) override { }

	virtual void Update(f32 dt) override { }

	virtual void PostUpdate(f32 dt) override { }

private:
	SystemManager* manager;

};

template <typename T>
const SystemTypeId System<T>::STATIC_SYSTEM__TYPE_ID = util::FamilyTypeId<ISystem>::get<T>();


} // namespace ecs
} // namespace raptor_engine