#include <cassert>

#include "memory/allocators/pool_allocator.hpp"

using namespace raptor_engine::memory::allocator;

pool_allocator::pool_allocator(std::size_t memory_size_,
							   const void* memory_,
							   std::size_t object_size_,
							   u8		   object_alignment_)
	: i_allocator {memory_size_, memory_}
	, object_size {object_size_}
	, object_alignment {object_alignment_}
{
	this->clear();
}

pool_allocator::~pool_allocator() 
{
	this->free_list = nullptr;
}

void* pool_allocator::allocate(std::size_t size_, u8 alignment_)
{
	assert(size_ > 0 && "allocate called with memroy_size = 0.");
	assert(size_ == this->object_size && alignment_ == this->object_alignment);

	if (this->free_list == nullptr)
	{
		return nullptr;
	}

	void* pointer = this->free_list;

	this->free_list = (void**)(*this->free_list);

	this->memory_used += this->object_size;
	this->memory_allocations_count++;

	return pointer;
}

void pool_allocator::free(void* memory_) 
{
	*((void**)memory_) = this->free_list;
	this->free_list	   = (void**)memory_;

	this->memory_used -= this->object_size;
	this->memory_allocations_count--;
}

void pool_allocator::clear() 
{
	u8 adjustment              = get_adjustment(this->memory_address, this->object_alignment);
	std::size_t number_objects = (std::size_t)floor((this->memory_size - adjustment) / this->object_size);

	union
	{
		void* as_void_pointer;
		uiptr as_uiptr;
	};

	as_void_pointer = (void*)this->memory_address;

	as_uiptr += adjustment;

	this->free_list = (void**)as_void_pointer;

	void** pointer = this->free_list;

	for (int i = 0; i < (number_objects - 1); ++i)
	{
		*pointer = (void*)((uiptr)pointer + this->object_size);
		pointer	 = (void**)*pointer;
	}

	*pointer = nullptr;
}
