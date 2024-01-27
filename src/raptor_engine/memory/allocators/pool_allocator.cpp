#include <cassert>

#include "memory/allocators/pool_allocator.hpp"

using namespace raptor_engine::memory::allocator;

PoolAllocator::PoolAllocator(std::size_t memorySize_,
							 const void* memory_,
							 std::size_t objectSize_,
							 u8 	     objectAlignment_)
	: IAllocator {memorySize_, memory_}
	, objectSize {objectSize_}
	, objectAlignment {objectAlignment_}
{
	this->Clear();
}

PoolAllocator::~PoolAllocator()
{
	this->freeList = nullptr;
}

void* PoolAllocator::Allocate(std::size_t size_, u8 alignment_)
{
	assert(size_ > 0 && "allocate called with memroy_size = 0.");
	assert(size_ == this->objectSize && alignment_ == this->objectAlignment);

	if (this->freeList == nullptr)
	{
		return nullptr;
	}

	void* pointer = this->freeList;

	this->freeList = (void**)(*this->freeList);

	this->memoryUsed += this->objectSize;
	this->memoryAllocationsCount++;

	return pointer;
}

void PoolAllocator::Free(void* memory_)
{
	*((void**)memory_) = this->freeList;
	this->freeList	   = (void**)memory_;

	this->memoryUsed -= this->objectSize;
	this->memoryAllocationsCount--;
}

void PoolAllocator::Clear()
{
	u8 adjustment             = GetAdjustment(this->memoryAddress, this->objectAlignment);
	std::size_t numberObjects = (std::size_t)floor((this->memorySize - adjustment) / this->objectSize);

	union
	{
		void* asVoidPointer;
		uiptr asUintPtr;
	};

	asVoidPointer = (void*)this->memoryAddress;

	asUintPtr += adjustment;

	this->freeList = (void**)asVoidPointer;

	void** pointer = this->freeList;

	for (int i = 0; i < (numberObjects - 1); ++i)
	{
		*pointer = (void*)((uiptr)pointer + this->objectSize);
		pointer	 = (void**)*pointer;
	}

	*pointer = nullptr;
}
