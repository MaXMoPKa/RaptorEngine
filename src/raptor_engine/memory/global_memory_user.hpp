#pragma once

#include "memory/memory_manager.hpp"

namespace raptor_engine {
namespace memory {

class global_memory_user
{
public:
	global_memory_user();
	virtual ~global_memory_user() = default;

public:
	const void* allocate(std::size_t memory_size_, const char* user = nullptr);
	void		free(void* memory_pointer_);

private:
	memory_manager* ecs_memory_manager;
};

}
}