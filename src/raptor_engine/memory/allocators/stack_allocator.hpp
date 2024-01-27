#pragma once

#include <cstddef>      // for size_t
#include <type_traits>  // for aligned_storage, aligned_storage<>::type, ali...

#include "defines.hpp"  // for u8

#include "memory/allocators/i_allocator.hpp"

namespace raptor_engine {
namespace memory {
namespace allocator {

class StackAllocator : public IAllocator
{
public:
	StackAllocator(const std::size_t memorySize_, const void* memory_);
	virtual ~StackAllocator() override;

public:
	virtual void* Allocate(const std::size_t size_, const u8 alignment_) override;
	virtual void  Free(void* memory_) override;
	virtual void  Clear() override;

private:
	struct AllocatorMetaInfo
	{
		u8 adjustment;
	};
};

}  // namespace allocator
}  // namespace memory
}  // namespace raptor_engine
