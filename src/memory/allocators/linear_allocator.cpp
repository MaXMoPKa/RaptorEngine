#include "linear_allocator.hpp"

#include "memory/utils.hpp"

#include <cassert>

using namespace raptor_engine::memory;
using namespace raptor_engine::memory::allocator;

linear_allocator::linear_allocator()
    : size { 0 }
    , address { nullptr }
    , used_size { 0 }
{
}

linear_allocator::linear_allocator(std::size_t size, const void* address)
    : size { size }
    , address { address }
    , used_size { 0 }
{
}

linear_allocator::linear_allocator(linear_allocator&& allocator) noexcept
{
    this->swap(allocator);
}

linear_allocator& linear_allocator::operator=(linear_allocator&& allocator)
{
    linear_allocator tmp(std::move(allocator));
    this->swap(tmp);
    return *this;
}

linear_allocator::~linear_allocator()
{
    this->size = 0;
    this->address = nullptr;
    this->used_size = 0;
}

void linear_allocator::create(std::size_t size, const void* address)
{
    linear_allocator tmp(size, address);
    this->swap(tmp);
}

void linear_allocator::swap(linear_allocator& allocator) noexcept
{
    if (this == &allocator) {
        return;
    }

    std::swap(this->size, allocator.size);
    std::swap(this->address, allocator.address);
    std::swap(this->used_size, allocator.used_size);
}

void linear_allocator::close() noexcept
{
    linear_allocator tmp {};
    this->swap(tmp);
}

void* linear_allocator::allocate(std::size_t size, u8 alignment)
{
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

bool linear_allocator::is_allocation_possible(std::size_t size)
{
    return this->size >= this->used_size + size;
}

void linear_allocator::free()
{
    assert(false && "Linear allocators doesn't support freeing of memory blocks. Call clear() instead.");
}

void linear_allocator::clear() noexcept
{
    this->used_size = 0;
}
