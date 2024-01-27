#pragma once

#include "memory/allocators/i_allocator.hpp"

#include "defines.hpp"

namespace raptor_engine {
namespace memory {
namespace allocator {

class PoolAllocator : public IAllocator
{
public:
	PoolAllocator(std::size_t memorySize_,
		          const void* memory_,
		          std::size_t objectSize_,
		          u8          objectAlignment_);

	virtual ~PoolAllocator();

public:
	virtual void* Allocate(std::size_t size_, u8 alignment_) override;
	virtual void  Free(void* memory_) override;
	virtual void  Clear() override;

private:
	const std::size_t objectSize;
	const u8		  objectAlignment;
	void**			  freeList;
};

}  // namespace allocator
}  // namespace memory
}  // namespace raptor_engine
