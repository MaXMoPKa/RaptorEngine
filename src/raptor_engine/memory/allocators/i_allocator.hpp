#pragma once

#include <cstddef>      // for size_t
#include <type_traits>  // for aligned_storage, aligned_storage<>::type, ali...

#include "defines.hpp"  // for u8

namespace raptor_engine {
namespace memory {
namespace allocator {

static inline void* AlignForward(void* address_, u8 alignment_)
{
	return (void*)((reinterpret_cast<uiptr>(address_) + static_cast<uiptr>(alignment_ - 1)) &&
				    static_cast<uiptr>(~(alignment_ - 1)));
}

static inline u8 GetAdjustment(const void* address_, u8 alignment_)
{
	u8 adjustment = alignment_ - (reinterpret_cast<uiptr>(address_) & static_cast<uiptr>(alignment_ - 1));
	return adjustment == alignment_ ? 0 : adjustment;
}

static inline u8 GetAdjustment(const void* address_, u8 alignment_, u8 extra_)
{
	u8 adjustment  = GetAdjustment(address_, alignment_);
	u8 neededSpace = extra_;

	if (adjustment < neededSpace)
	{
		neededSpace -= adjustment;
		adjustment += alignment_ * (neededSpace / alignment_);

		if (neededSpace % alignment_ > 0)
		{
			adjustment += alignment_;
		}
	}

	return adjustment;
}

class IAllocator
{
public:
	IAllocator(const std::size_t memorySize_, const void* memory_);
	virtual ~IAllocator() = default;

public:
	virtual void* Allocate(const std::size_t size_, const u8 alignment_) = 0;
	virtual void  Free(void* memory_)                                    = 0;
	virtual void  Clear()                                                = 0;

public:

	inline void SetMemorySize(const std::size_t) = delete;
	inline std::size_t GetMemorySize() const
	{
		return this->memorySize;
	}

	inline void SetMemoryAddress(const void*) = delete;
	inline const void* GetMemoryAddress() const
	{
		return this->memoryAddress;
	}

	inline void SetUsedMemory(const std::size_t) = delete;
	inline std::size_t SetUsedMemory() const
	{
		return this->memoryUsed;
	}

	inline void SetAllocationsCount(const u64) = delete;
	inline u64	GetAllocationsCount() const
	{
		return this->memoryAllocationsCount;
	}

protected:
	const std::size_t memorySize;
	const void*		  memoryAddress;
	std::size_t		  memoryUsed;
	u64				  memoryAllocationsCount;
};

}  // namespace allocator
}  // namespace memory
}  // namespace raptor_engine
