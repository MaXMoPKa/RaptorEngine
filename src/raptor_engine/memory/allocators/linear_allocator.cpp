#include <cassert>  // for assert
#include <utility>  // for swap, move

#include "linear_allocator.hpp"

#include "memory/utils.hpp"  // for get_adjustment
#include "utils/simpl.hpp"

using namespace raptor_engine::utils;
using namespace raptor_engine::memory;
using namespace raptor_engine::memory::allocator;

LinearAllocator::LinearAllocator(const std::size_t memorySize_, const void* memory_) 
	: IAllocator(memorySize_, memory_)
{ }

LinearAllocator::~LinearAllocator()
{
	this->Clear();
}

void* LinearAllocator::Allocate(const std::size_t size_, const u8 alignment_)
{
	assert(size_ > 0 && "allocate calles with memSize = 0.");

	union
	{
		void* asVoidPointer;
		uiptr  asUptr;
	};

	asVoidPointer = (void*)this->memoryAddress;
	asUptr += this->memoryUsed;

	u8 adjustment = GetAdjustment(asVoidPointer, alignment_);

	if (this->memoryUsed + size_ + adjustment > this->memorySize) {
		return nullptr;
	}

	asUptr += adjustment;

	this->memoryUsed += size_ + adjustment;
	this->memoryAllocationsCount++;

	return asVoidPointer;
}

void LinearAllocator::Free(void* memory_)
{
	assert(false && "Linear allocators do not support free operations. Use clear unstead.");
}

void LinearAllocator::Clear()
{
	this->memoryUsed			 = 0;
	this->memoryAllocationsCount = 0;
}
