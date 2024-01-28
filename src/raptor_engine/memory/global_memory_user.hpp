#pragma once

#include <cstddef>

namespace raptor_engine {
namespace memory {

class GlobalMemoryUser
{
public:

	GlobalMemoryUser();
	virtual ~GlobalMemoryUser() = default;

public:
	const void* Allocate(std::size_t memorySize_, const char* user_ = nullptr);
	void		Free(void* memoryPointer_);
};

}
}