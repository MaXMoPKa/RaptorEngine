#pragma once

#include <cstddef>      // for size_t
#include <type_traits>  // for aligned_storage, aligned_storage<>::type, ali...

#include "defines.hpp"  // for u8

#include "memory/allocators/i_allocator.hpp"

namespace raptor_engine {
namespace memory {
namespace allocator {

class stack_allocator : public i_allocator
{
public:
	stack_allocator(const std::size_t memory_size_, const void* memory);
	virtual ~stack_allocator() override;

public:
	virtual void* allocate(const std::size_t size_, const u8 alignment_) override;
	virtual void  free(void* memory_) override;
	virtual void  clear() override;

private:
	struct allocator_meta_info
	{
		u8 adjustment;
	};
};

}  // namespace allocator
}  // namespace memory
}  // namespace raptor_engine
