#include "linear_allocator.hpp"

#include <cassert>  // for assert
#include <utility>  // for swap, move

#include "memory/utils.hpp"  // for get_adjustment

using namespace raptor_engine::memory;
using namespace raptor_engine::memory::allocator;

class linear_allocator::linear_allocator_pimpl {
 public:
  linear_allocator_pimpl() : size{0}, address{nullptr}, used_size{0} {}

  linear_allocator_pimpl(std::size_t size, const void* address)
      : size{size}, address{address}, used_size{0} {}

  ~linear_allocator_pimpl() {
    this->used_size = 0;
    this->size = 0;
    this->address = nullptr;
  }

 public:
  void create(std::size_t size, const void* address) {
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
  [[nodiscard]] void* allocate(std::size_t size, u8 alignment) {
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

  [[nodiscard]] inline std::size_t get_size() const { return this->size; }

  [[nodiscard]] inline std::size_t get_used_size() const {
    return this->used_size;
  }

 public:
  bool is_allocation_possible(std::size_t size) {
    return this->size >= this->used_size + size;
  }

 private:
  std::size_t size;
  const void* address;
  std::size_t used_size;
};

linear_allocator::linear_allocator() : pimpl{new linear_allocator_pimpl()} {}

linear_allocator::linear_allocator(std::size_t size, const void* address)
    : pimpl{new linear_allocator_pimpl(size, address)} {}

linear_allocator::linear_allocator(linear_allocator&& allocator) noexcept =
    default;

linear_allocator& linear_allocator::operator=(
    linear_allocator&& allocator) noexcept = default;

linear_allocator::~linear_allocator() {}

void linear_allocator::create(std::size_t size, const void* address) {
  this->pimpl->create(size, address);
}

void linear_allocator::swap(linear_allocator& allocator) noexcept {
  this->pimpl->swap(*allocator.pimpl);
}

void linear_allocator::reset() noexcept { this->pimpl->reset(); }

void* linear_allocator::allocate(std::size_t size, u8 alignment) {
  return this->pimpl->allocate(size, alignment);
}

void linear_allocator::free() { this->pimpl->free(); }

void linear_allocator::clear() noexcept { this->pimpl->clear(); }

bool linear_allocator::is_allocation_possible(std::size_t size) {
  return this->pimpl->is_allocation_possible(size);
}

[[nodiscard]] void const* linear_allocator::get_address() const {
  return this->pimpl->get_address();
}

[[nodiscard]] std::size_t linear_allocator::get_size() const {
  return this->pimpl->get_size();
}

[[nodiscard]] std::size_t linear_allocator::get_used_size() const {
  return this->pimpl->get_used_size();
}
