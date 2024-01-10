#include <cassert>  // for assert
#include <utility>  // for swap, move

#include "memory/allocators/stack_allocator.hpp"

using namespace raptor_engine::memory::allocator;

stack_allocator::stack_allocator(const std::size_t memory_size_, const void* memory_) 
	: i_allocator {memory_size_, memory_} 
{

}

stack_allocator::~stack_allocator()
{
	this->clear();
}

void* stack_allocator::allocate(const std::size_t size_, const u8 alignment_)
{
	assert(size_ > 0 && "allocate called with memory_size == 0");

	union
	{
		void*				 as_void_pointer;
		uiptr				 as_uiptr;
		allocator_meta_info* as_meta_info;
	};

	as_void_pointer = (void*)this->memory_address;

	as_uiptr += this->memory_used;

	u8 adjustment = get_adjustment(as_void_pointer, alignment_, sizeof(allocator_meta_info));

	if (this->memory_used + size_ + adjustment > this->memory_size)
	{
		return nullptr;
	}

	as_meta_info->adjustment = adjustment;

	as_uiptr += adjustment;

	this->memory_used += size_ + adjustment;
	this->memory_allocations_count++;

	return as_void_pointer;
}

void stack_allocator::free(void* memory_)
{
	union
	{
		void* as_void_pointer;
		uiptr as_uiptr;
		allocator_meta_info* as_meta_info;
	};

	as_void_pointer = memory_;

	as_uiptr -= sizeof(allocator_meta_info);

	this->memory_used -=
		((uiptr)this->memory_address + this->memory_used) - ((uiptr)memory_ + as_meta_info->adjustment);
	this->memory_allocations_count--;
}

void stack_allocator::clear()
{
	this->memory_used              = 0;
	this->memory_allocations_count = 0;
}