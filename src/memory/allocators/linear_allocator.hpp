#pragma once

#include <cstddef>  // for size_t
#include <memory>   // for unique_ptr

#include "defines.hpp"  // for u8

namespace raptor_engine {
namespace memory {
namespace allocator {

class linear_allocator {
  static constexpr u8 default_alignment = 64U;

 public:
  linear_allocator();

  linear_allocator(std::size_t size, void const* address);

  linear_allocator(const linear_allocator&) = delete;
  linear_allocator& operator=(const linear_allocator&) = delete;

  linear_allocator(linear_allocator&& allocator) noexcept;
  linear_allocator& operator=(linear_allocator&& allocator) noexcept;

  ~linear_allocator();

 public:
  void create(std::size_t size, void const* address);

  void swap(linear_allocator& allocator) noexcept;

  void reset() noexcept;

 public:
  [[nodiscard]] bool is_allocation_possible(std::size_t size);

 public:
  [[nodiscard]] void* allocate(std::size_t size,
                               u8 alignment = default_alignment);

  void free();

  void clear() noexcept;

 public:
  [[nodiscard]] void const* get_address() const;

  [[nodiscard]] std::size_t get_size() const;

  [[nodiscard]] std::size_t get_used_size() const;

 private:
  class linear_allocator_pimpl;
  std::unique_ptr<linear_allocator_pimpl> pimpl;
};

}  // namespace allocator
}  // namespace memory
}  // namespace raptor_engine
