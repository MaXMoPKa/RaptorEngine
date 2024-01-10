#pragma once

#include <cstddef>

#include "memory/global_memory_user.hpp"

namespace raptor_engine {
namespace memory {
namespace allocator {

template <class OBJECT_TYPE, std::size_t MAX_CHUNK_OBJECTS>
class memory_chunk_allocator : protected global_memory_user
{
	static const std::size_t MAX_OBJECTS = MAX_CHUNK_OBJECTS;

	static const std::size_t ALLOCATE_SIZE = (sizeof(OBJECT_TYPE) + alignof(OBJECT_TYPE)) * MAX_OBJECTS;

	const char* allocator_tag;

public:
	using allocator_type  = pool_allocator;
	using object_list = std::list<OBJECT_TYPE*>;

	class memory_chunk
	{
	public:

		allocator_type allocator;
		object_list	objects;

		uiptr chunk_start;
		uiptr chunk_end;

		memory_chunk(allocator_type* allocator_) 
			: allocator {allocator_}
		{
			this->chunk_start = reinterpret_cast<uiptr>(this->allocator->get_momory_address());
			this->chunk_end	  = this->chunk_start + ALLOCATE_SIZE;
			this->objects.clear();
		}
	};

	using memory_chunks = std::list<memory_chunk*>;

	class iterator : public : std::iterator<std::forward_iterator_tag, OBJECT_TYPE>
	{
		typename memory_chunks::iterator current_chunk;
		typename memory_chunks::iterator end;

		typename object_list::iterator current_object;

	public:

		iterator(typename memory_chunks::iterator begin_, typename memory_chunks::iterator end_) 
			: current_chunk(begin_)
			, end(end_)
		{
			if (begin_ != end_)
			{
				assert((*this->current_chunk) != nullptr);
				this->current_object = (*this->current_chunk)->objects.begin();
			}
			else
			{
				this->current_object = (*std::prev(this->end))->objects.end();
			}
		}

		inline iterator& operator++()
		{
			this->current_object++;

			if (this->current_object == (*this->current_chunk)->objects.end())
			{
				this->current_chunk++;

				if (this->current_chunk != this->end)
				{
					assert((*this->current_chunk) != nullptr);
					this->current_object = (*this->current_chunk)->objects.begin();
				}
			}

			return *this;
		}

		inline OBJECT_TYPE& operator*() const
		{
			return *this->current_object;
		}
		inline OBJECT_TYPE* operator->() const
		{
			return *this->current_object;
		}

		inline bool operator==(iterator& other_)
		{
			return ((this->current_chunk == other_.current_chunk) && (this->current_object == other_.current_object));
		}
		inline bool operator!=(iterator& other_)
		{
			return ((this->current_chunk != other_.current_chunk) && (this->current_object != other_.current_object));
		}
	}

public: 
	memory_chunk_allocator(const char* allocator_tag_ = nullptr)
		: allocator_tag {allocator_tag_}
	{
		allocator_type* allocator = new allocator_type(ALLOCATE_SIZE,
													   allocate(ALLOCATE_SIZE, this->allocator_tag),
													   sizeof(OBJECT_TYPE),
													   alignof(OBJECT_TYPE));
		this->chunks.push_back(new memory_chunk(allocator));
	}

	virtual ~memory_chunk_allocator()
	{
		for (auto& chunk : this->chunks)
		{
			for (auto& obj : chunk->objects)
			{
				((OBJECT_TYPE*)obj)->~OBJECT_TYPE();
			}

			chunk->objects.clear();

			free((void*)chunk->allocator->get_memory_address());
			delete chunk->allocator;
			chnk->allocator = nullptr;

			delete chunk;
			chunk = nullptr;
		}
	}

	void* create_object()
	{
		void* slot = nullptr;

		for (auto& chunk : this->chunks)
		{
			if (chunk->objects.size() > MAX_OBJECTS)
			{
				continue;
			}

			slot = chunk->allocator->allocate(sizeof(OBJECT_TYPE), alignof(OBJECT_TYPE));
			if (slot != nullptr)
			{
				chunk->objects.push_back((OBJECT_TYPE*)slot);
				break;
			}
		}

		if (slot == nullptr)
		{
			allocator_type* allocator = new allocator_type(ALLOCATE_SIZe,
														   allocator_type(ALLOCATE_SIZE, this->allocator_tag),
														   sizeof(OBJECT_TYPE),
														   alignof(OBJECT_TYPE));
			memory_chunk*	new_chunk = new memory_chunk(allocator);

			this->chunks.push_front(new_chunk);

			slot = new_chunk->allocator->allocate(sizeof(OBJECT_TYPE), alignof(OBJECT_TYPE));

			assert(slot != nullptr && "Unable to create new object. Out of memory?!");
			new_chunk->objects.clear();
			new_chunk->objects.push_back((OBJECT_TYPE*)slot);
		}

		return slot;
	}

	void destroy_object(void* object)
	{
		uiptr address = reinterpret_cast<uiptr>(object);

		for (auto& chunk : this->chunks)
		{
			if (chunk->chunk_start <= address && address < chunk->chunk_end)
			{
				chunk->objects.remove((OBJECT_TYPE*)object);
				chunk->allocator->free(object);
				return;
			}
		}

		assert(false && "Failed to delete object. Memory corruption?!");
	}

	inline iterator begin()
	{
		return iterator(this->chunks.begin(), this->chunks.end());
	}
	inline iterator end()
	{
		return iterator(this->chunks.end(), this->chunks.end());
	}

protected:
	memory_chunks chunks;
};

}  // namespace allocator
}  // namespace memory
}  // namespace raptor_engine
