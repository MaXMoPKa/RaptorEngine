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
class linear_allocator {
  static constexpr u8 default_alignment = 64U;  // default alignment is 64 bit.

 public:
  /**
   * @brief Construct a new linear allocator object with 0 memory size and
   * memory address equal nullptr. For initialization use @see create(u64 size,
   * void const* address) method.
   *
   */
  linear_allocator();

  /**
   * @brief Construct a new linear allocator object with transfered data.
   *
   * @param size is a size of memory for storage in bytes.
   * @param address is the pointer on first bytes of storage memory.
   */
  linear_allocator(u64 size, void const* address);

  linear_allocator(const linear_allocator&) = delete;
  linear_allocator& operator=(const linear_allocator&) = delete;

  /**
   * @brief Construct a new linear allocator object from another
   * linear_allocator object.
   */
  linear_allocator(linear_allocator&& allocator) noexcept;
  linear_allocator& operator=(linear_allocator&& allocator) noexcept;

  ~linear_allocator();

 public:
  /**
   * @brief create is one way to initiaize linear_allocator object.
   *
   * @param size is a size of memory for storage in bytes.
   * @param address is the pointer on first bytes of storage memory.
   */
  void create(u64 size, void const* address);

  /**
   * @brief swap is the method for swap two linear_allocator data (pointer on
   * storage, its size and used memory)
   */
  void swap(linear_allocator& allocator) noexcept;

  /**
   * @brief reset method exchange pointer on memory on nullptr wuth zero storage
   * memory size.
   *
   */
  void reset() noexcept;

 public:
  /**
   * @return true if allocation of memory block with size is possible, and false
   * in another way.
   */
  [[nodiscard]] bool is_allocation_possible(u64 size);

 public:
  /**
   * @brief allocate is method for allocation of new memory block with @p size
   * and @p alignment.
   *
   * @param size of memory block for allocation.
   * @param alignment of new allocated memory block, by default it is 64bit.
   * @return void* on new allocated memory block.
   */
  [[nodiscard]] void* allocate(u64 size, u8 alignment = default_alignment);

  /**
   * @brief @warning linear allocators doesn't support freeing of memory blocks.
   * Call @see clear() instead.
   *
   */
  void free();

  /**
   * @brief clear method forgot about all objects used its memory.
   * @warning clear method doesn't free memory and doesn't destray objects in
   * used memory.
   *
   */
  void clear() noexcept;

 public:
  /**
   * @return the raw pointer on the first byte of storage memory.
   */
  [[nodiscard]] void const* get_address() const;

  /**
   * @return the storage size in bytes.
   */
  [[nodiscard]] u64 get_size() const;

  /**
   * @return the used memory size of storage in bytes.
   */
  [[nodiscard]] u64 get_used_size() const;

 private:
  static constexpr u64 pimpl_size =
      sizeof(u64) + sizeof(void const*) + sizeof(u64);
  static constexpr u64 pimpl_alignment =
      std::alignment_of<std::max_align_t>::value;
  using aligned_storage_type =
      std::aligned_storage<pimpl_size, pimpl_alignment>::type;
  aligned_storage_type pimpl;
};

}  // namespace allocator
}  // namespace memory
}  // namespace raptor_engine
