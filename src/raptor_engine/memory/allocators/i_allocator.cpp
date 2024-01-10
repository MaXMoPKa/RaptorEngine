#include <cassert>  // for assert
#include <utility>  // for swap, move

#include "memory/allocators/i_allocator.hpp"

using namespace raptor_engine::memory::allocator;

i_allocator::i_allocator(const std::size_t memory_size_, const void* memory_) 
	: memory_size {memory_size_}
	, memory_address {memory_}
	, memory_used {0}
	, memory_allocations_count {0}
{

}