#include "memory/allocators/linear_allocator.hpp"

#include <gmock/gmock.h>
#include <memory>

using namespace testing;
using namespace raptor_engine::memory::allocator;

class test_linear_allocator : public Test {
protected:
    void SetUp()
    {
        address = std::make_unique<u8[]>(size);
        allocator.create(size, address.get());
    }

    void TearDown()
    {
        allocator.close();
    }

    linear_allocator allocator;

    std::size_t const size { 16 };
    std::unique_ptr<u8[]> address;
};

TEST(linear_allocator, create_with_nullptr_and_zero_memory_with_parametrized_constructor)
{
    linear_allocator allocator { 0, nullptr };

    ASSERT_THAT(allocator.get_address(), Eq(nullptr));
    ASSERT_THAT(allocator.get_size(), Eq(0));
}

TEST(linear_allocator, create_with_default_values)
{
    linear_allocator allocator;

    ASSERT_THAT(allocator.get_address(), Eq(nullptr));
    ASSERT_THAT(allocator.get_size(), Eq(0));
}

TEST(linear_allocator, swap_empty_linear_allocator_and_allocator_with_valid_values)
{
    std::size_t const size { 16 };
    auto address = std::make_unique<u8[]>(size);

    linear_allocator non_zero_allocator;
    non_zero_allocator.create(size, address.get());

    linear_allocator zero_allocator;

    non_zero_allocator.swap(zero_allocator);

    ASSERT_THAT(zero_allocator.get_address(), Eq(address.get()));
    ASSERT_THAT(zero_allocator.get_size(), Eq(size));
    ASSERT_THAT(non_zero_allocator.get_address(), Eq(nullptr));
    ASSERT_THAT(non_zero_allocator.get_size(), Eq(0));
}

TEST(linear_allocator, create_allocator_using_move_constructor)
{
    std::size_t const size { 16 };
    auto address = std::make_unique<u8[]>(size);

    std::function<linear_allocator(std::size_t const, void const*)> test_function =
        [](std::size_t const size, void const* address) -> linear_allocator {
        // std:move is used for prohibit RVO/NRVO
        return std::move(linear_allocator { size, address });
    };

    linear_allocator allocator = test_function(size, address.get());

    ASSERT_THAT(allocator.get_address(), Eq(address.get()));
    ASSERT_THAT(allocator.get_size(), Eq(size));
}

TEST(linear_allocator, create_allocator_using_move_assignment)
{
    std::size_t const size { 16 };
    auto address = std::make_unique<u8[]>(size);

    linear_allocator first_allocator { size, address.get() };
    linear_allocator second_allocator;

    second_allocator = std::move(first_allocator);

    ASSERT_THAT(second_allocator.get_address(), Eq(address.get()));
    ASSERT_THAT(second_allocator.get_size(), Eq(size));
}

TEST(linear_allocator, create_with_create_method_with_nullptr_and_zero_memory)
{
    std::size_t const size { 16 };
    auto address = std::make_unique<u8[]>(size);

    linear_allocator allocator;
    allocator.create(size, address.get());

    ASSERT_THAT(allocator.get_address(), Eq(address.get()));
    ASSERT_THAT(allocator.get_size(), Eq(size));
}

TEST_F(test_linear_allocator, clear_allocated_memory_using_close_method)
{
    allocator.close();

    ASSERT_THAT(allocator.get_address(), Eq(nullptr));
    ASSERT_THAT(allocator.get_size(), Eq(0));
}

TEST_F(test_linear_allocator, check_that_allocation_is_possible)
{
    std::size_t const allocation_size { 4 };
    auto const is_allocation_possible = allocator.is_allocation_possible(allocation_size);

    ASSERT_THAT(is_allocation_possible, Eq(true));
}

TEST_F(test_linear_allocator, allocate_bytes_of_memory)
{
    u8 const alignment = 4U;

    std::size_t const first_allocation_size { 8 };
    void const* first_allocated_memory { allocator.allocate(first_allocation_size, alignment) };

    std::size_t const second_allocation_size { 4 };
    void const* second_allocated_memory { allocator.allocate(second_allocation_size, alignment) };

    ASSERT_THAT(allocator.get_address(), Eq(address.get()));
    ASSERT_THAT(allocator.get_size(), Eq(size));
    ASSERT_THAT(allocator.get_used_size(), Eq(first_allocation_size + second_allocation_size));
    ASSERT_THAT(first_allocated_memory, Eq(address.get()));
    ASSERT_THAT(second_allocated_memory, Eq(address.get() + first_allocation_size));
}

TEST_F(test_linear_allocator, release_all_occupied_memory_completely_inside_allocator)
{
    allocator.clear();

    ASSERT_THAT(allocator.get_address(), Eq(address.get()));
    ASSERT_THAT(allocator.get_size(), Eq(size));
    ASSERT_THAT(allocator.get_used_size(), Eq(0));
}

TEST_F(test_linear_allocator, allocate_twelve_bytes_with_alignment_at_8)
{
    std::size_t const allocation_size { 12 };
    u8 const allocation_alignment = 8;
    void const* allocated_memory_ptr { allocator.allocate(allocation_size, allocation_alignment) };

    u8 const adjustment = allocation_alignment - (allocation_size & (allocation_alignment - 1));
    ASSERT_THAT(allocated_memory_ptr, Eq(address.get() + adjustment));
    ASSERT_THAT(allocator.get_size(), Eq(size));
    ASSERT_THAT(allocator.get_used_size(), Eq(allocation_size + adjustment));
}