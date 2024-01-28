#include <memory/global_memory_user.hpp>

#include "memory/memory_manager.hpp"

using namespace raptor_engine::memory;

GlobalMemoryUser::GlobalMemoryUser() = default;

const void* GlobalMemoryUser::Allocate(std::size_t memorySize_, const char* user_)
{
	return MemoryManager::GetInstance()->Allocate(memorySize_, user_);
}

void GlobalMemoryUser::Free(void* memoryPointer_)
{
	MemoryManager::GetInstance()->Free(memoryPointer_);
}