#include <cassert>  // for assert
#include <utility>  // for swap, move

#include "linear_allocator.hpp"

#include "memory/utils.hpp"  // for get_adjustment
#include "utils/simpl.hpp"

using namespace raptor_engine::utils;
using namespace raptor_engine::memory;
using namespace raptor_engine::memory::allocator;

class linear_allocator_pimpl {
 public:
  linear_allocator_pimpl() : size{0}, address{nullptr}, used_size{0} {}

  linear_allocator_pimpl(u64 size, const void* address)
      : size{size}, address{address}, used_size{0} {}

  ~linear_allocator_pimpl() {
    this->used_size = 0;
    this->size = 0;
    this->address = nullptr;
  }

 public:
  void create(u64 size, const void* address) {
    linear_allocator_pimpl tmp(size, address);
    this->swap(tmp);
  }

  void swap(linear_allocator_pimpl& allocator) noexcept {
    if (this == &allocator) {
      return;
    }

    std::swap(this->size, allocator.size);
    std::swap(this->address, allocator.address);
    std::swap(this->used_size, allocator.used_size);
  }

  void reset() noexcept {
    linear_allocator_pimpl tmp{};
    this->swap(tmp);
  }

 public:
  [[nodiscard]] void* allocate(u64 size, u8 alignment) {
    union {
      void* as_void_ptr;
      uiptr as_uiptr;
    };

    as_void_ptr = (void*)this->address;
    as_uiptr += this->used_size;

    auto const adjustment = utils::get_adjustment(size, alignment);

    if (!this->is_allocation_possible(size + adjustment)) {
      return nullptr;
    }

    as_uiptr += adjustment;

    this->used_size += size + adjustment;

    return as_void_ptr;
  }

  void free() {
    assert(false &&
           "Linear allocators doesn't support freeing of memory blocks. Call "
           "clear() instead.");
  }

  void clear() noexcept { this->used_size = 0; }

 public:
  [[nodiscard]] inline void const* get_address() const { return this->address; }

  [[nodiscard]] inline u64 get_size() const { return this->size; }

  [[nodiscard]] inline u64 get_used_size() const { return this->used_size; }

 public:
  bool is_allocation_possible(u64 size) {
    return this->size >= this->used_size + size;
  }

 private:
  u64 size;
  const void* address;
  u64 used_size;
};

linear_allocator::linear_allocator() {
  static_assert(sizeof(pimpl) >= sizeof(linear_allocator_pimpl),
                "buffer not big enough to hold linear_allocator_pimpl");

  static_assert(
      std::alignment_of<aligned_storage_type>::value >=
          std::alignment_of<linear_allocator_pimpl>::value,
      "alignment requirements of linear_allocator_pimpl not fulfilled");

  placement_new<linear_allocator_pimpl>(&(this->pimpl), sizeof(this->pimpl));
}

linear_allocator::linear_allocator(u64 size, const void* address) {
  static_assert(sizeof(pimpl) >= sizeof(linear_allocator_pimpl),
                "buffer not big enough to hold linear_allocator_pimpl");

  static_assert(
      std::alignment_of<aligned_storage_type>::value >=
          std::alignment_of<linear_allocator_pimpl>::value,
      "alignment requirements of linear_allocator_pimpl not fulfilled");

  placement_new<linear_allocator_pimpl>(&(this->pimpl), sizeof(this->pimpl),
                                        size, address);
}

linear_allocator::linear_allocator(linear_allocator&& allocator) noexcept =
    default;

linear_allocator& linear_allocator::operator=(
    linear_allocator&& allocator) noexcept = default;

linear_allocator::~linear_allocator() {
  placement_delete<linear_allocator_pimpl>(&this->pimpl);
}

void linear_allocator::create(u64 size, const void* address) {
  placement_cast<linear_allocator_pimpl>(&this->pimpl)->create(size, address);
}

void linear_allocator::swap(linear_allocator& allocator) noexcept {
  auto allocator_pimpl =
      placement_cast<linear_allocator_pimpl>(&allocator.pimpl);
  placement_cast<linear_allocator_pimpl>(&this->pimpl)->swap(*allocator_pimpl);
}

void linear_allocator::reset() noexcept {
  placement_cast<linear_allocator_pimpl>(&this->pimpl)->reset();
}

void* linear_allocator::allocate(u64 size, u8 alignment) {
  return placement_cast<linear_allocator_pimpl>(&this->pimpl)
      ->allocate(size, alignment);
}

void linear_allocator::free() {
  placement_cast<linear_allocator_pimpl>(&this->pimpl)->free();
}

void linear_allocator::clear() noexcept {
  placement_cast<linear_allocator_pimpl>(&this->pimpl)->clear();
}

bool linear_allocator::is_allocation_possible(u64 size) {
  return placement_cast<linear_allocator_pimpl>(&this->pimpl)
      ->is_allocation_possible(size);
}

[[nodiscard]] void const* linear_allocator::get_address() const {
  return placement_cast<linear_allocator_pimpl>(&this->pimpl)->get_address();
}

[[nodiscard]] u64 linear_allocator::get_size() const {
  return placement_cast<linear_allocator_pimpl>(&this->pimpl)->get_size();
}

[[nodiscard]] u64 linear_allocator::get_used_size() const {
  return placement_cast<linear_allocator_pimpl>(&this->pimpl)->get_used_size();
}
