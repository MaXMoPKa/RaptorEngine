#include <cassert>  // for assert
#include <utility>  // for swap, move

#include "memory/allocators/stack_allocator.hpp"

using namespace raptor_engine::memory::allocator;

StackAllocator::StackAllocator(const std::size_t memorySize_, const void* memory_) 
	: IAllocator {memorySize_, memory_} 
{

}

StackAllocator::~StackAllocator()
{
	this->Clear();
}

void* StackAllocator::Allocate(const std::size_t size_, const u8 alignment_)
{
	assert(size_ > 0 && "allocate called with memory_size == 0");

	union
	{
		void*			   asVoidPointer;
		uiptr			   asUintPtr;
		AllocatorMetaInfo* asMetaInfo;
	};

	asVoidPointer = (void*)this->memoryAddress;

	asUintPtr += this->memoryUsed;

	u8 adjustment = GetAdjustment(asVoidPointer, alignment_, sizeof(AllocatorMetaInfo));

	if (this->memoryUsed + size_ + adjustment > this->memorySize)
	{
		return nullptr;
	}

	asMetaInfo->adjustment = adjustment;

	asUintPtr += adjustment;

	this->memoryUsed += size_ + adjustment;
	this->memoryAllocationsCount++;

	return asVoidPointer;
}

void StackAllocator::Free(void* memory_)
{
	union
	{
		void* asVoidPointer;
		uiptr asUintPtr;
		AllocatorMetaInfo* asMetaInfo;
	};

	asVoidPointer = memory_;

	asUintPtr -= sizeof(AllocatorMetaInfo);

	this->memoryUsed -=
		((uiptr)this->memoryAddress + this->memoryUsed) - ((uiptr)memory_ + asMetaInfo->adjustment);
	this->memoryAllocationsCount--;
}

void StackAllocator::Clear()
{
	this->memoryUsed             = 0;
	this->memoryAllocationsCount = 0;
}