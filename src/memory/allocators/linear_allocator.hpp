#pragma once

#include <cstddef>      // for size_t
#include <type_traits>  // for aligned_storage, aligned_storage<>::type, ali...

#include "defines.hpp"  // for u8

namespace raptor_engine {
namespace memory {
namespace allocator {

class linear_allocator {
  static constexpr u8 default_alignment = 64U;

 public:
  linear_allocator();

  linear_allocator(u64 size, void const* address);

  linear_allocator(const linear_allocator&) = delete;
  linear_allocator& operator=(const linear_allocator&) = delete;

  linear_allocator(linear_allocator&& allocator) noexcept;
  linear_allocator& operator=(linear_allocator&& allocator) noexcept;

  ~linear_allocator();

 public:
  void create(u64 size, void const* address);

  void swap(linear_allocator& allocator) noexcept;

  void reset() noexcept;

 public:
  [[nodiscard]] bool is_allocation_possible(u64 size);

 public:
  [[nodiscard]] void* allocate(u64 size, u8 alignment = default_alignment);

  void free();

  void clear() noexcept;

 public:
  [[nodiscard]] void const* get_address() const;

  [[nodiscard]] u64 get_size() const;

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
