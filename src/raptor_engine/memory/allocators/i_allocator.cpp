#include <cassert>  // for assert
#include <utility>  // for swap, move

#include "memory/allocators/i_allocator.hpp"

using namespace raptor_engine::memory::allocator;

IAllocator::IAllocator(const std::size_t memorySize_, const void* memory_) 
	: memorySize {memorySize_}
	, memoryAddress {memory_}
	, memoryUsed {0}
	, memoryAllocationsCount {0}
{

}