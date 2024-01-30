#pragma once

#include <cstddef>      // for size_t
#include <type_traits>  // for aligned_storage, aligned_storage<>::type, ali...

#include "defines.hpp"  // for u8

namespace raptor_engine {
namespace memory {
namespace allocator {

/**
 * @brief linear_allocator is the class representing the simplest implementation
 * of linear allocator with alignment.
 *
 */
class LinearAllocator {
  static constexpr u8 defaultAlignment = 64U;  // default alignment is 64 bit.

 public:
  /**
   * @brief Construct a new linear allocator object with 0 memory size and
   * memory address equal nullptr. For initialization use @see create(u64 size,
   * void const* address) method.
   *
   */
  LinearAllocator();

  /**
   * @brief Construct a new linear allocator object with transfered data.
   *
   * @param size is a size of memory for storage in bytes.
   * @param address is the pointer on first bytes of storage memory.
   */
  LinearAllocator(u64 size_, void const* address_);

  LinearAllocator(const LinearAllocator&)			 = delete;
  LinearAllocator& operator=(const LinearAllocator&) = delete;

  /**
   * @brief Construct a new linear allocator object from another
   * linear_allocator object.
   */
  LinearAllocator(LinearAllocator&& allocator_) noexcept;
  LinearAllocator& operator=(LinearAllocator&& allocator_) noexcept;

  ~LinearAllocator();

 public:
  /**
   * @brief create is one way to initiaize linear_allocator object.
   *
   * @param size is a size of memory for storage in bytes.
   * @param address is the pointer on first bytes of storage memory.
   */
  void Create(u64 size_, void const* address_);

  /**
   * @brief swap is the method for swap two linear_allocator data (pointer on
   * storage, its size and used memory)
   */
  void Swap(LinearAllocator& allocator_) noexcept;

  /**
   * @brief reset method exchange pointer on memory on nullptr wuth zero storage
   * memory size.
   *
   */
  void Reset() noexcept;

 public:
  /**
   * @return true if allocation of memory block with size is possible, and false
   * in another way.
   */
  [[nodiscard]] bool IsAllocationPossible(u64 size_);

 public:
  /**
   * @brief allocate is method for allocation of new memory block with @p size
   * and @p alignment.
   *
   * @param size of memory block for allocation.
   * @param alignment of new allocated memory block, by default it is 64bit.
   * @return void* on new allocated memory block.
   */
  [[nodiscard]] void* Allocate(u64 size_, u8 alignment_ = defaultAlignment);

  /**
   * @brief @warning linear allocators doesn't support freeing of memory blocks.
   * Call @see clear() instead.
   *
   */
  void Free();

  /**
   * @brief clear method forgot about all objects used its memory.
   * @warning clear method doesn't free memory and doesn't destray objects in
   * used memory.
   *
   */
  void Clear() noexcept;

 public:
  /**
   * @return the raw pointer on the first byte of storage memory.
   */
  [[nodiscard]] void const* GetAddress() const;

  /**
   * @return the storage size in bytes.
   */
  [[nodiscard]] u64 GetSize() const;

  /**
   * @return the used memory size of storage in bytes.
   */
  [[nodiscard]] u64 GetUsedSize() const;

 private:
  static constexpr u64 pimplSize =
      sizeof(u64) + sizeof(void const*) + sizeof(u64);
  static constexpr u64 pimplAlignment =
      std::alignment_of<std::max_align_t>::value;
  using AlignedStorageType =
      std::aligned_storage<pimplSize, pimplAlignment>::type;
  AlignedStorageType pimpl;
};

using LinearAllocatorUptr = std::unique_ptr<LinearAllocator>;
using LinearAllocatorSptr = std::shared_ptr<LinearAllocator>;
using LinearAllocatorWptr = std::weak_ptr<LinearAllocator>;

}  // namespace allocator
}  // namespace memory
}  // namespace raptor_engine
