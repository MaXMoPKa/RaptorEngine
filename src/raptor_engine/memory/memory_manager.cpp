#include <memory/memory_manager.hpp>

#include <algorithm>
#include <cassert>

#include "defines.hpp"

#include "SDL2/SDL_log.h"

using namespace raptor_engine::memory;

MemoryManager::MemoryManager()
{
	this->globalMemory = malloc(MemoryManager::MEMORY_CAPACITY);
	assert(this->globalMemory != nullptr && "Failed to allocate global memory.");

	this->memoryAllocator = new StackAllocator(MemoryManager::MEMORY_CAPACITY, this->globalMemory);
	assert(this->memoryAllocator != nullptr && "Failed to create memory allocator!");

	this->pendingMemory.clear();
	this->freedMemory.clear();
}

MemoryManager::~MemoryManager()
{
	this->memoryAllocator->Clear();
	delete this->memoryAllocator;
	this->memoryAllocator = nullptr;

	free(this->globalMemory);
	this->globalMemory = nullptr;
}

void* MemoryManager::Allocate(std::size_t memorySize_, const std::string& user_)
{
	void* pointMemory = memoryAllocator->Allocate(memorySize_, alignof(u8));
	this->pendingMemory.push_back(std::make_pair(user_, pointMemory));
	return pointMemory;
}

void MemoryManager::Free(void* pointerMemory_)
{
	if (pointerMemory_ == this->pendingMemory.back().second)
	{
		this->memoryAllocator->Free(pointerMemory_);
		this->pendingMemory.pop_back();

		bool check = true;
		while (check == true)
		{
			check = false;
			const auto& it =
				std::find_if(this->freedMemory.begin(), 
					         this->freedMemory.end(), 
					         [&](const void* pointer_) 
			                 {
					             return pointer_ == this->pendingMemory.back().second;
				             });
			if (it != this->freedMemory.end())
			{
				this->memoryAllocator->Free(pointerMemory_);
				this->pendingMemory.pop_back();
				this->freedMemory.remove(*it);

				check = true;
			}
		}
	}
	else
	{
		this->freedMemory.push_back(pointerMemory_);
	}
}

void MemoryManager::CheckMemoryLeaks()
{
	assert(!(this->freedMemory.size() > 0 && this->pendingMemory.size() == 0) && "Implementation failure");

	if (this->pendingMemory.size() > 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR::MEMORY_MANAGER: memory leak detected!");
		for (const auto& i : this->pendingMemory)
		{
			bool isFreed = false;
			isFreed	  = std::any_of(this->freedMemory.begin(), 
				                        this->freedMemory.end(), 
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
			if (isFreed == false)
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