#include <memory/memory_manager.hpp>

#include <algorithm>

#include "defines.hpp"

#include "SDL2/SDL_log.h"

using namespace raptor_engine::memory;

memory_manager::memory_manager()
{
	this->global_memory = malloc(memory_manager::MEMORY_CAPACITY);
	assert(this->global_memory != nullptr && "Failed to allocate global memory.");

	this->memory_allocator = new stack_allocator(memory_manager::MEMORY_CAPACITY, this->global_memory);
	assert(this->memory_allocator != nullptr && "Failed to create memory allocator!");

	this->pending_memory.clear();
	this->freed_memory.clear();
}

memory_manager::~memory_manager()
{
	this->memory_allocator->clear();
	delete this->memory_allocator;
	this->memory_allocator = nullptr;

	free(this->global_memory);
	this->global_memory = nullptr;
}

void* memory_manager::allocate(std::size_t memory_size_, const std::string& user_)
{
	void* point_memory = memory_allocator->allocate(memory_size_, alignof(u8));
	this->pending_memory.push_back(std::make_pair(user_, point_memory));
	return point_memory;
}

void memory_manager::free(void* pointer_memory_)
{
	if (pointer_memory_ == this->pending_memory.back().second)
	{
		this->memory_allocator->free(pointer_memory_);
		this->pending_memory.pop_back();

		bool check = true;
		while (check == true)
		{
			check = false;
			const auto& it =
				std::find_if(this->freed_memory.begin(), 
					         this->freed_memory.end(), 
					         [&](const void* pointer_) 
			                 {
					             return pointer_ == this->pending_memory.back().second;
				             });
			if (it != this->freed_memory.end())
			{
				this->memory_allocator->free(pointer_memory_);
				this->pending_memory.pop_back();
				this->freed_memory.remove(*it);

				check = true;
			}
		}
	}
	else
	{
		this->freed_memory.push_back(pointer_memory_);
	}
}

void memory_manager::check_memory_leaks()
{
	assert(!(this->freed_memory.size() > 0 && this->pending_memory.size() == 0) && "Implementation failure");

	if (this->pending_memory.size() > 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR::MEMORY_MANAGER: memory leak detected!");
		for (const auto& i : this->pending_memory)
		{
			bool is_freed = false;
			is_freed	  = std::any_of(this->freed_memory.begin(), 
				                        this->freed_memory.end(), 
				                        [&i](const void* pointer_) 
			                            { 
				                            if (pointer_ == i.second)
			                                {
					                            return true;
											}
											else
											{
												return false;
											}
			                            });
			if (is_freed == false)
			{
				SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR::MEMORY_MANAGER: %s memory user didn't release allocated memory %s", i.first.c_str(), i.second);
			}
		}
	}
	else
	{
		SDL_LogInfo(SDL_LOG_CATEGORY_ERROR, "INFO::MEMORY_MANAGER: no memory leaks detected.");
	}
}