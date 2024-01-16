#pragma once

#include "ecs/i_system.hpp"

#include "memory/global_memory_user.hpp"
#include "memory/allocators/linear_allocator.hpp"

#include <unordered_map>
#include <cassert>

namespace raptor_engine {
namespace ecs {

using system_work_state_mask = std::vector<bool>;

class system_manager : public memory::global_memory_user
{
	using system_allocator         = memory::allocator::linear_allocator;
	using system_registry          = std::unordered_map<u64, i_system*>;
	using system_dependency_matrix = std::vector<std::vector<bool>>;
	using system_work_order        = std::vector<i_system*>;

public:
	system_manager();

	system_manager(const system_manager&)			 = delete;
	system_manager& operator=(const system_manager&) = delete;

	~system_manager();

public:
	template <typename T, class... ARGS>
	T* add_system(ARGS&&... args)
	{
		const u64 static_system_type_id = T::STATIC_SYSTEM_TYPE_ID;

		auto it = this->registry.find(static_system_type_id);
		if ((it != this->registry.end()) && (it->second != nullptr))
		{
			return (T*)it->second;
		}

		T* system = nullptr;
		void* system_mem = this->allocator->allocate(sizeof(T), alignof(T));
		if (system_mem != nullptr)
		{
			((T*)system_mem)->system_manager = this;

			system                                = new (system_mem) T(std::forward<ARGS>(args)...);
			this->registry[static_system_type_id] = system;

			// LogInfo("System \'%s\' (%d bytes) created.", typeid(T).name(), sizeof(T));
		}
		else
		{
			// LogError("Unable to create system \'%s\' (%d bytes).", typeid(T).name(), sizeof(T));
			assert(true);
		}

		if (static_system_type_id + 1 > this->dependency_matrix.size())
		{
			this->dependency_matrix.resize(static_system_type_id + 1);
			for (std::size_t i = 0; i < this->dependency_matrix.size(); ++i)
			{
				this->dependency_matrix[i].resize(static_system_type_id + 1);
			}
		}

		this->work_order.push_back(system);

		return system;
	}

	template <typename System, class Dependency>
	void add_system_dependency(System target_, Dependency dependency_)
	{
		const u64 TARGET_ID = target_->get_static_system_type_id();
		const u64 DEPEND_ID = dependency_->get_static_system_type_id();

		if (this->dependency_matrix[TARGET_ID][DEPEND_ID] != true) {
			this->dependency_matrix[TARGET_ID][DEPEND_ID] = true;
			// LogInfo("added '%s' as dependency to '%s'", dependency->GetSystemTypeName(), target->GetSystemTypeName())
		}

		// this->UpdateSystemWorkOrder();
	}

	template <typename Target, class Dependency, class... Dependencies>
	void add_system_dependency(Target target_, Dependency dependency_, Dependencies&&... dependencies_)
	{
		const u64 TARGET_ID = target_->get_static_system_type_id();
		const u64 DEPEND_ID = dependency_->get_static_system_type_id();

		if (this->dependency_matrix[TARGET_ID][DEPEND_ID] != true) {
			this->dependency_matrix[TARGET_ID][DEPEND_ID] = true;
			// LogInfo("added '%s' as dependency to '%s'", dependency->GetSystemTypeName(), target->GetSystemTypeName())
		}

		this->add_system_dependency(target_, std::forward<Dependencies>(dependencies_)...);
	}

	void update_system_work_order();

	template <typename T>
	inline T* get_system() const
	{
		auto it    = this->registry.find(T::STATIC_SYSTEM_TYPE_ID);
		return it != this->registry.end() ? (T*)it->second : nullptr;
	}

	template <typename T>
	void enable_system()
	{
		const system_type_id STID = T::STATIC_SYSTEM_TYPE_ID;

		auto it = this->registry.find(STID);
		if (it != this->registry.end())
		{
			if (it->second->is_enabled == true)
			{
				return;
			}

			it->second->is_enabled = true;
		}
		else
		{
			// LogWarning("Trying to enable system [%d], but system is not "
			//            "registered yet.",
            //            STID);
		}
	}

	template <typename T>
	void disable_system()
	{
		const system_type_id STID = T::STATIC_SYSTEM_TYPE_ID;

		auto it = this->registry.find(STID);
		if (it != this->registry.end()) 
		{ 
			if (it->second->is_enabled == false)
			{
				return;
			}

			it->second->is_enabled = false;
		}
		else
		{
			// LogWarning("Trying to disable system [%d], but system is not "
			//	          "registered yet.",
            //            STID);
		}
	}

	template <typename T>
	void set_system_update_interval(f32 interval_ms_)
	{
		const system_type_id STID = T::STATIC_SYSTEM_TYPE_ID;

		auto it = this->registry.find(STID);
		if (it != this->registry.end())
		{
			it->second->update_interval = interval_ms_;
		}
		else
		{
			// LogWarning("Trying to change system's [%d] update interval, but "
			//	    	  "system is not registered yet.",
            //            STID);
		}
	}

	template <typename T>
	void set_system_priority(system_priority new_priority_)
	{
		const system_type_id STID = T::STATIC_SYSTEM_TYPE_ID;

		auto it = this->registry.find(STID);
		if (it != this->registry.end())
		{ 
		    system_priority old_priority = it->second->priority;

			if(old_priority == new_priority_)
			{
				return;
			}

			it->second->priority = new_priority_;

			// re-build system work order
			// this->UpdateSystemWorkOrder();
		}
		else
		{
			// LogWarning("Trying to change system's [%d] priority, but system is "
			//		      "not registered yet.",
            //            STID);
		}
	}

	system_work_state_mask get_system_work_state() const;

	void set_system_work_state(const system_work_state_mask& mask);

	template <typename... active_systems>
	system_work_state_mask generate_active_system_work_state(active_systems&&... active_systems_)
	{
		system_work_state_mask mask {this->work_order.size(), false};
		std::list<i_system*>   as = {active_systems_...};

		for (auto s : as)
		{
			for (std::size_t i = 0; i < this->work_order.size(); ++i)
			{
				if (this->work_order[i]->get_static_system_type_id() == s->get_static_system_type_id())
				{
					mask[i] = true;
					break;
				}
			}
		}
		return mask;
	}

private:
	void update(f32 dt);

private:

	system_allocator*        allocator;
	system_registry	         registry;
	system_dependency_matrix dependency_matrix;
	system_work_order		 work_order;
};

} // namespace ecs
} // namespace raptor_engine