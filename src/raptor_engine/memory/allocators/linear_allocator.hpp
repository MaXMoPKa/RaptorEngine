#pragma once

#include <cstddef>      // for size_t
#include <type_traits>  // for aligned_storage, aligned_storage<>::type, ali...

#include "defines.hpp"  // for u8

#include "memory/allocators/i_allocator.hpp"

namespace raptor_engine {
namespace memory {
namespace allocator {

class LinearAllocator : public IAllocator
{
  /*
   *   Allocates memory in a linear way
   *
   *       first          2    3     4
   *     allocation        allocation
   *         v            v    v     v
   * |=================|=====|===|=======|...|
   * ^                                       ^
   * Initial                                 Last possible
   * memory                                  memory address
   * address                                 (memory + memorySize)
   * (memory)
   */

public:
    LinearAllocator(const std::size_t memorySize_, const void* memory_);
    virtual ~LinearAllocator();

public:
    virtual void* Allocate(const std::size_t size_, const u8 alignment_) override;
    virtual void  Free(void* memory_) override;
    virtual void  Clear() override;
};

using LinearAllocatorUptr = std::unique_ptr<LinearAllocator>;
using LinearAllocatorSptr = std::shared_ptr<LinearAllocator>;
using LinearAllocatorWptr = std::weak_ptr<LinearAllocator>;

}  // namespace allocator
}  // namespace memory
}  // namespace raptor_engine
