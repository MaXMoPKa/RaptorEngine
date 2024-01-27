#pragma once

#include "ecs/system/i_system.hpp"

#include "memory/global_memory_user.hpp"
#include "memory/allocators/linear_allocator.hpp"

#include <unordered_map>
#include <cassert>

namespace raptor_engine {
namespace ecs {

using SystemWorkStateMask = std::vector<bool>;

class SystemManager : public memory::GlobalMemoryUser
{
	friend class Engine;

	using SystemAllocator        = memory::allocator::LinearAllocator;
	using SystemRegistry         = std::unordered_map<u64, ISystem*>;
	using SystemDependencyMatrix = std::vector<std::vector<bool>>;
	using SystemWorkOrder        = std::vector<ISystem*>;

public:
	SystemManager();

	SystemManager(const SystemManager&)            = delete;
	SystemManager& operator=(const SystemManager&) = delete;

	~SystemManager();

public:
	template <typename T, class... ARGS>
	T* AddSystem(ARGS&&... args_)
	{
		const u64 staticSystemTypeId = T::STATIC_SYSTEM_TYPE_ID;

		auto it = this->registry.find(staticSystemTypeId);
		if ((it != this->registry.end()) && (it->second != nullptr))
		{
			return (T*)it->second;
		}

		T* system = nullptr;
		void* systemMemory = this->allocator->Allocate(sizeof(T), alignof(T));
		if (systemMemory != nullptr)
		{
			((T*)systemMemory)->systemManager = this;

			system							   = new (systemMemory) T(std::forward<ARGS>(args_)...);
			this->registry[staticSystemTypeId] = system;

			// LogInfo("System \'%s\' (%d bytes) created.", typeid(T).name(), sizeof(T));
		}
		else
		{
			// LogError("Unable to create system \'%s\' (%d bytes).", typeid(T).name(), sizeof(T));
			assert(true);
		}

		if (staticSystemTypeId + 1 > this->dependencyMatrix.size())
		{
			this->dependencyMatrix.resize(staticSystemTypeId + 1);
			for (std::size_t i = 0; i < this->dependencyMatrix.size(); ++i)
			{
				this->dependencyMatrix[i].resize(staticSystemTypeId + 1);
			}
		}

		this->workOrder.push_back(system);

		return system;
	}

	template <typename System, class Dependency>
	void AddSystemDependency(System target_, Dependency dependency_)
	{
		const u64 TARGET_ID = target_->GetStaticSystemTypeId();
		const u64 DEPEND_ID = dependency_->GetStaticSystemTypeId();

		if (this->dependencyMatrix[TARGET_ID][DEPEND_ID] != true) {
			this->dependencyMatrix[TARGET_ID][DEPEND_ID]  = true;
			// LogInfo("added '%s' as dependency to '%s'", dependency->GetSystemTypeName(), target->GetSystemTypeName())
		}

		// this->UpdateSystemWorkOrder();
	}

	template <typename Target, class Dependency, class... Dependencies>
	void AddSystemDependency(Target target_, Dependency dependency_, Dependencies&&... dependencies_)
	{
		const u64 TARGET_ID = target_->GetStaticSystemTypeId();
		const u64 DEPEND_ID = dependency_->GetStaticSystemTypeId();

		if (this->dependencyMatrix[TARGET_ID][DEPEND_ID] != true) {
			this->dependencyMatrix[TARGET_ID][DEPEND_ID] = true;
			// LogInfo("added '%s' as dependency to '%s'", dependency->GetSystemTypeName(), target->GetSystemTypeName())
		}

		this->AddSystemDependency(target_, std::forward<Dependencies>(dependencies_)...);
	}

	void UpdateSystemWorkOrder();

	template <typename T>
	inline T* GetSystem() const
	{
		auto it    = this->registry.find(T::STATIC_SYSTEM_TYPE_ID);
		return it != this->registry.end() ? (T*)it->second : nullptr;
	}

	template <typename T>
	void EnableSystem()
	{
		const SystemTypeId staticSystemTypeId = T::STATIC_SYSTEM_TYPE_ID;

		auto it = this->registry.find(staticSystemTypeId);
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
            //            staticSystemTypeId);
		}
	}

	template <typename T>
	void DisableSystem()
	{
		const SystemTypeId staticSystemTypeId = T::STATIC_SYSTEM_TYPE_ID;

		auto it = this->registry.find(staticSystemTypeId);
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
            //            staticSystemTypeId);
		}
	}

	template <typename T>
	void SetSystemUpdateInterval(f32 intervalMs_)
	{
		const SystemTypeId staticSystemTypeId = T::STATIC_SYSTEM_TYPE_ID;

		auto it = this->registry.find(staticSystemTypeId);
		if (it != this->registry.end())
		{
			it->second->update_interval = intervalMs_;
		}
		else
		{
			// LogWarning("Trying to change system's [%d] update interval, but "
			//	    	  "system is not registered yet.",
            //            staticSystemTypeId);
		}
	}

	template <typename T>
	void SetSystemPriority(SystemPriority newPriority_)
	{
		const SystemTypeId staticSystemTypeId = T::STATIC_SYSTEM_TYPE_ID;

		auto it = this->registry.find(staticSystemTypeId);
		if (it != this->registry.end())
		{ 
		    SystemPriority oldPriority = it->second->priority;

			if(oldPriority == newPriority_)
			{
				return;
			}

			it->second->priority = newPriority_;

			// re-build system work order
			// this->UpdateSystemWorkOrder();
		}
		else
		{
			// LogWarning("Trying to change system's [%d] priority, but system is "
			//		      "not registered yet.",
            //            staticSystemTypeId);
		}
	}

	SystemWorkStateMask GetSystemWorkState() const;

	void SetSystemWorkState(const SystemWorkStateMask& mask_);

	template <typename... ActiveSystems>
	SystemWorkStateMask GenerateActiveSystemWorkState(ActiveSystems&&... activeSystems_)
	{
		SystemWorkStateMask mask {this->workOrder.size(), false};
		std::list<ISystem*>   activeSystems = {activeSystems_...};

		for (auto system : activeSystems)
		{
			for (std::size_t i = 0; i < this->workOrder.size(); ++i)
			{
				if (this->workOrder[i]->GetStaticSystemTypeId() == system->GetStaticSystemTypeId())
				{
					mask[i] = true;
					break;
				}
			}
		}
		return mask;
	}

private:
	void Update(f32 dt_);

private:

	SystemAllocator*       allocator;
	SystemRegistry	       registry;
	SystemDependencyMatrix dependencyMatrix;
	SystemWorkOrder		   workOrder;
};

} // namespace ecs
} // namespace raptor_engine