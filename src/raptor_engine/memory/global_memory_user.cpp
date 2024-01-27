#include <memory/global_memory_user.hpp>

using namespace raptor_engine::memory;

GlobalMemoryUser::GlobalMemoryUser() 
	: ecsMemoryManager(new MemoryManager()) 
{

}

const void* GlobalMemoryUser::Allocate(std::size_t memorySize_, const char* user_)
{
	return this->ecsMemoryManager->Allocate(memorySize_, user_);
}

void GlobalMemoryUser::Free(void* memoryPointer_)
{
	this->ecsMemoryManager->Free(memoryPointer_);
}