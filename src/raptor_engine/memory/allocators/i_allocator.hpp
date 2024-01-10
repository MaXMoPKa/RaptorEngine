#pragma once

#include <cstddef>      // for size_t
#include <type_traits>  // for aligned_storage, aligned_storage<>::type, ali...

#include "defines.hpp"  // for u8

namespace raptor_engine {
namespace memory {
namespace allocator {

static inline void* align_forward(void* address_, u8 alignment_)
{
	return (void*)((reinterpret_cast<uiptr>(address_) + static_cast<uiptr>(alignment_ - 1)) &&
				    static_cast<uiptr>(~(alignment_ - 1)));
}

static inline u8 get_adjustment(const void* address_, u8 alignment_)
{
	u8 adjustment = alignment_ - (reinterpret_cast<uiptr>(address_) & static_cast<uiptr>(alignment_ - 1));
	return adjustment == alignment_ ? 0 : adjustment;
}

static inline u8 get_adjustment(const void* address_, u8 alignment_, u8 extra_)
{
	u8 adjustment   = get_adjustment(address_, alignment_);
	u8 needed_space = extra_;

	if (adjustment < needed_space)
	{
		needed_space -= adjustment;
		adjustment += alignment_ * (needed_space / alignment_);

		if (needed_space % alignment_ > 0)
		{
			adjustment += alignment_;
		}
	}

	return adjustment;
}

class i_allocator
{
public:
	i_allocator(const std::size_t memory_size_, const void* memory);
	virtual ~i_allocator() = default;

public:
	virtual void* allocate(const std::size_t size_, const u8 alignment_) = 0;
	virtual void  free(void* memory_)                                    = 0;
	virtual void  clear()                                                = 0;

public:

	inline void set_memory_size(const std::size_t) = delete;
	inline std::size_t get_memory_size() const
	{
		return this->memory_size;
	}

	inline void set_memory_address(const void*) = delete;
	inline const void* get_memory_address() const
	{
		return this->memory_address;
	}

	inline void set_used_memory(const std::size_t) = delete;
	inline std::size_t set_used_memory() const
	{
		return this->memory_used;
	}

	inline void set_allocations_count(const u64) = delete;
	inline u64	get_allocations_count() const
	{
		return this->memory_allocations_count;
	}

protected:
	const std::size_t memory_size;
	const void*		  memory_address;
	std::size_t		  memory_used;
	u64				  memory_allocations_count;
};

}  // namespace allocator
}  // namespace memory
}  // namespace raptor_engine
