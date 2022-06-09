#pragma once

#include "defines.hpp"

#include <utility>

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

            linear_allocator& operator=(linear_allocator&& allocator);

            ~linear_allocator();

        public:
            void create(std::size_t size, void const* address);

            void swap(linear_allocator& allocator) noexcept;

            void close() noexcept;

        public:
            [[nodiscard]] void* allocate(std::size_t size, u8 alignment = default_alignment);

            bool is_allocation_possible(std::size_t size);

            void free();

            void clear() noexcept;

        public:
            [[nodiscard]] inline void const* get_address() const
            {
                return this->address;
            }

            [[nodiscard]] inline std::size_t get_size() const
            {
                return this->size;
            }

            [[nodiscard]] inline std::size_t get_used_size() const
            {
                return this->used_size;
            }

        private:
            std::size_t size;
            const void* address;
            std::size_t used_size;
        };

    } // namespace allocator
} // namespace memory
} // namespace raptor_engine
