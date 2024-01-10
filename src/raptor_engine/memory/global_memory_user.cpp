#include <memory/global_memory_user.hpp>

using namespace raptor_engine::memory;

global_memory_user::global_memory_user() 
	: ecs_memory_manager(new memory_manager()) 
{

}

const void* global_memory_user::allocate(std::size_t memory_size_, const char* user_)
{
	return this->ecs_memory_manager->allocate(memory_size_, user_);
}

void global_memory_user::free(void* memory_pointer_)
{
	this->ecs_memory_manager->free(memory_pointer_);
}