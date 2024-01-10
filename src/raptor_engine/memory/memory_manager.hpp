#pragma once

#include <string>
#include <vector>
#include <list>
#include <utility>

#include "memory/allocators/stack_allocator.hpp"

#define ECS_GLOBAL_MEMORY_CAPACITY 134217728 // 128 MB

namespace raptor_engine {
namespace memory {

class memory_manager
{
	using stack_allocator = allocator::stack_allocator;

public:
	memory_manager();
	~memory_manager();

	memory_manager(const memory_manager&)			 = delete;
	memory_manager& operator=(const memory_manager&) = delete;

public:
	void* allocate(std::size_t memory_size, const std::string& user = "");

	void free(void* point_memory);

	void check_memory_leaks();

public:
	static constexpr std::size_t MEMORY_CAPACITY = ECS_GLOBAL_MEMORY_CAPACITY;

public:
	void*                                            global_memory;
	stack_allocator*                                 memory_allocator;
	std::vector<std::pair<const std::string, void*>> pending_memory;
	std::list<void*>								 freed_memory;
};

}
}