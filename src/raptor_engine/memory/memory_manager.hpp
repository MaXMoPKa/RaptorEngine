#pragma once

#include <string>
#include <vector>
#include <list>
#include <utility>

#include "memory/allocators/stack_allocator.hpp"

#define ECS_GLOBAL_MEMORY_CAPACITY 134217728 // 128 MB

namespace raptor_engine {
namespace memory {

class MemoryManager
{
	using StackAllocator = allocator::StackAllocator;

public:
	static constexpr std::size_t MEMORY_CAPACITY = ECS_GLOBAL_MEMORY_CAPACITY;

public:
	MemoryManager();
	~MemoryManager();

	MemoryManager(const MemoryManager&)			   = delete;
	MemoryManager& operator=(const MemoryManager&) = delete;

public:
	void* Allocate(std::size_t memorySize_, const std::string& user_ = "");

	void Free(void* pointMemory_);

	void CheckMemoryLeaks();

public:
	static MemoryManager* GetInstance()
	{
		if (managerInstance == nullptr) 
		{
			managerInstance = new MemoryManager();
		}

		return managerInstance;
	}

private:
	static MemoryManager* managerInstance;

private:
	void*                                            globalMemory;
	StackAllocator*                                  memoryAllocator;
	std::vector<std::pair<const std::string, void*>> pendingMemory;
	std::list<void*>								 freedMemory;
};

}
}