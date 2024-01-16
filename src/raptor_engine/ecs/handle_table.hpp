#pragma once

#include "ecs/handle.hpp"

#include <vector>

namespace raptor_engine {
namespace ecs {
namespace util
{

template <class T, class handle_type, std::size_t grow = 1024>
class handle_table
{
public:
	handle_table()
	{
		this->grow_table();
	}

	~handle_table() = default;

public:
	handle_type acquire_handle(T* raw_object_)
	{
		typename handle_type::value_type i = 0;
		for (; i < this->table.size(); ++i)
		{
			if (this->table[i].second == nullptr)
			{
				this->table[i].second = raw_object_;
				this->table[i].first  = ((this->table[i].first + 1) > handle_type::MAX_VERSION) ?
											handle_type::MAX_VERSION :
											this->table[i].first + 1;
				return handle_type(i, this->table[i].first);
			}
		}

		this->grow_table();

		this->table[i].first  = 1;
		this->table[i].second = raw_object_;

		return handle_type(i, this->table[i].first);
	}

	void release_handle(handle_type handle_)
	{
		assert((handle_.index < this->table.size() && handle_.version == this->table[handle_.index].first) &&
			   "Invalid hande!");
		this->table[handle_.index].second = nullptr;
	}

	inline bool is_expired(handle_type handle_) const
	{
		return this->table[handle_.index].first != handle_.version;
	}

	inline handle_type operator[](typename handle_type::value_type index_) const
	{
		assert(index_ < this->table.size() && "Invalid handle!");
		return handle_type(index_, this->table[index_].first);
	}

	inline T* operator[](handle_type handle_)
	{
		assert((handle_.index < this->table.size() && handle_.version == this->table[handle_.index].first) &&
			   "Invalid handle!");
		return (this->table[handle_.index].first == handle_.version ? this->table[handle_.index].second : nullptr);
	}

private:
	void grow_table()
	{
		std::size_t old_size = this->table.size();

		assert(old_size < handle_type::MAX_INDICES && "Max table capacity reached!");

		std::size_t new_size = std::min(old_size + grow, (std::size_t)handle_type::MAX_INDICES);

		this->table.resize(new_size);

		for (typename handle_type::value_type i = old_size; i < new_size; ++i)
		{
			this->table[i] = table_entry(handle_type::MIN_VERSION, nullptr);
		}
	}

private:
	using table_entry = std::pair<typename handle_type::value_type, T*>;

	std::vector<table_entry> table;
};


} // namespace util
} // namespace ecs
} // namespace raptor_engine