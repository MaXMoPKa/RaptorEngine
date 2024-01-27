#pragma once

#include <cstddef>
#include <cassert>

#include "memory/global_memory_user.hpp"
#include "memory/allocators/pool_allocator.hpp"

namespace raptor_engine {
namespace memory {
namespace allocator {

template <class OBJECT_TYPE, std::size_t MAX_CHUNK_OBJECTS>
class MemoryChunkAllocator : protected GlobalMemoryUser
{
	static const std::size_t MAX_OBJECTS = MAX_CHUNK_OBJECTS;

	static const std::size_t ALLOCATE_SIZE = (sizeof(OBJECT_TYPE) + alignof(OBJECT_TYPE)) * MAX_OBJECTS;

	const char* allocatorTag;

public:
	using AllocatorType  = PoolAllocator;
	using ObjectList     = std::list<OBJECT_TYPE*>;

	class MemoryChunk
	{
	public:

		AllocatorType allocator;
		ObjectList	  objects;

		uiptr chunkStart;
		uiptr chunkEnd;

		MemoryChunk(AllocatorType* allocator_) 
			: allocator {allocator_}
		{
			this->chunkStart = reinterpret_cast<uiptr>(this->allocator->GetMemoryAddress());
			this->chunkEnd	 = this->chunkStart + ALLOCATE_SIZE;
			this->objects.clear();
		}
	};

	using MemoryChunks = std::list<MemoryChunk*>;

	class iterator : public std::iterator<std::forward_iterator_tag, OBJECT_TYPE>
	{
		typename MemoryChunks::iterator currentChunk;
		typename MemoryChunks::iterator end;

		typename ObjectList::iterator currentObject;

	public:

		iterator(typename MemoryChunks::iterator begin_, typename MemoryChunks::iterator end_)
			: currentChunk(begin_), end(end_)
		{
			if (begin_ != end_) 
			{
				assert((*this->currentChunk) != nullptr);
				this->currentObject = (*this->currentChunk)->objects.begin();
			} 
			else 
			{
				this->currentObject = (*std::prev(this->end))->objects.end();
			}
		}

		inline iterator& operator++()
		{
			this->currentObject++;

			if (this->currentObject == (*this->currentChunk)->objects.end()) {
				this->currentChunk++;

				if (this->currentChunk != this->end) {
					assert((*this->currentChunk) != nullptr);
					this->currentObject = (*this->currentChunk)->objects.begin();
				}
			}

			return *this;
		}

		inline OBJECT_TYPE& operator*() const
		{
			return *this->currentObject;
		}
		inline OBJECT_TYPE* operator->() const
		{
			return *this->currentObject;
		}

		inline bool operator==(iterator& other_)
		{
			return ((this->currentChunk == other_.currentChunk) && (this->currentObject == other_.currentObject));
		}
		inline bool operator!=(iterator& other_)
		{
			return ((this->currentChunk != other_.currentChunk) && (this->currentObject != other_.currentObject));
		}
	};

public: 
	MemoryChunkAllocator(const char* allocatorTag_ = nullptr)
		: allocatorTag {allocatorTag_}
	{
		AllocatorType* allocator = new AllocatorType(ALLOCATE_SIZE,
													 allocate(ALLOCATE_SIZE, this->allocatorTag),
													 sizeof(OBJECT_TYPE),
													 alignof(OBJECT_TYPE));
		this->chunks.push_back(new MemoryChunk(allocator));
	}

	virtual ~MemoryChunkAllocator()
	{
		for (auto& chunk : this->chunks)
		{
			for (auto& object : chunk->objects)
			{
				((OBJECT_TYPE*)object)->~OBJECT_TYPE();
			}

			chunk->objects.clear();

			free((void*)chunk->allocator->GetMemoryAddress());
			delete chunk->allocator;
			chunk->allocator = nullptr;

			delete chunk;
			chunk = nullptr;
		}
	}

	void* CreateObject()
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
			AllocatorType* allocator = new AllocatorType(ALLOCATE_SIZE,
														 AllocatorType(ALLOCATE_SIZE, this->allocatorTag),
														 sizeof(OBJECT_TYPE),
														 alignof(OBJECT_TYPE));
			MemoryChunk*	newChunk = new MemoryChunk(allocator);

			this->chunks.push_front(newChunk);

			slot = newChunk->allocator->Allocate(sizeof(OBJECT_TYPE), alignof(OBJECT_TYPE));

			assert(slot != nullptr && "Unable to create new object. Out of memory?!");
			newChunk->objects.clear();
			newChunk->objects.push_back((OBJECT_TYPE*)slot);
		}

		return slot;
	}

	void DestroyObject(void* object)
	{
		uiptr address = reinterpret_cast<uiptr>(object);

		for (auto& chunk : this->chunks)
		{
			if (chunk->chunkStart <= address && address < chunk->chunkEnd)
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
	MemoryChunks chunks;
};

}  // namespace allocator
}  // namespace memory
}  // namespace raptor_engine
