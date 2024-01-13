#pragma once

#include "memory/allocators/i_allocator.hpp"

#include "defines.hpp"

namespace raptor_engine {
namespace memory {
namespace allocator {

class pool_allocator : public i_allocator
{
public:
	pool_allocator(std::size_t memory_size_,
		           const void* memory_,
		           std::size_t object_size_,
		           u8 object_alignment_);

	virtual ~pool_allocator();

public:
	virtual void* allocate(std::size_t size_, u8 alignment_) override;
	virtual void  free(void* memory_) override;
	virtual void  clear() override;

private:
	const std::size_t object_size;
	const u8		  object_alignment;
	void**			  free_list;
};

}  // namespace allocator
}  // namespace memory
}  // namespace raptor_engine
