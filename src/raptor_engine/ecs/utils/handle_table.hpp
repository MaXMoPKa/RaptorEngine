#pragma once

#include "ecs/utils/handle.hpp"

#include <vector>

namespace raptor_engine {
namespace ecs {
namespace util
{

template <class T, class HandleType, std::size_t grow = 1024>
class HandleTable
{
public:
	HandleTable()
	{
		this->GrowTable();
	}

	~HandleTable() = default;

public:
	HandleType AcquireHandle(T* rawObject_)
	{
		typename HandleType::value_type i = 0;
		for (; i < this->table.size(); ++i)
		{
			if (this->table[i].second == nullptr)
			{
				this->table[i].second = rawObject_;
				this->table[i].first  = ((this->table[i].first + 1) > HandleType::MAX_VERSION) ?
											HandleType::MAX_VERSION :
											this->table[i].first + 1;
				return HandleType(i, this->table[i].first);
			}
		}

		this->GrowTable();

		this->table[i].first  = 1;
		this->table[i].second = rawObject_;

		return HandleType(i, this->table[i].first);
	}

	void ReleaseHandle(HandleType handleType_)
	{
		assert(
			(handleType_.index < this->table.size() && handleType_.version == this->table[handleType_.index].first) &&
			   "Invalid hande!");
		this->table[handleType_.index].second = nullptr;
	}

	inline bool IsExpired(HandleType handleType_) const
	{
		return this->table[handleType_.index].first != handleType_.version;
	}

	inline HandleType operator[](typename HandleType::value_type index_) const
	{
		assert(index_ < this->table.size() && "Invalid handle!");
		return HandleType(index_, this->table[index_].first);
	}

	inline T* operator[](HandleType handleType_)
	{
		assert(
			(handleType_.index < this->table.size() && handleType_.version == this->table[handleType_.index].first) &&
			   "Invalid handle!");
		return (this->table[handleType_.index].first == handleType_.version ? this->table[handleType_.index].second :
																		  nullptr);
	}

private:
	void GrowTable()
	{
		std::size_t oldSize = this->table.size();

		assert(oldSize < HandleType::MAX_INDICES && "Max table capacity reached!");

		std::size_t newSize = std::min(oldSize + grow, (std::size_t)HandleType::MAX_INDICES);

		this->table.resize(newSize);

		for (typename HandleType::value_type i = oldSize; i < newSize; ++i)
		{
			this->table[i] = TableEntry(HandleType::MIN_VERSION, nullptr);
		}
	}

private:
	using TableEntry = std::pair<typename HandleType::value_type, T*>;

	std::vector<TableEntry> table;
};


} // namespace util
} // namespace ecs
} // namespace raptor_engine