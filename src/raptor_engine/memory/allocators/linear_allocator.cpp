#include <cassert>  // for assert
#include <utility>  // for swap, move

#include "linear_allocator.hpp"

#include "memory/utils.hpp"  // for get_adjustment
#include "utils/simpl.hpp"

using namespace raptor_engine::utils;
using namespace raptor_engine::memory;
using namespace raptor_engine::memory::allocator;

class LinearAllocatorPimpl {
 public:
  LinearAllocatorPimpl() : size {0}, address {nullptr}, usedSize {0} { }

  LinearAllocatorPimpl(u64 size_, const void* address_)
      : size{size_}, address{address_}, usedSize{0} {}

  ~LinearAllocatorPimpl()
  {
    this->usedSize = 0;
    this->size = 0;
    this->address = nullptr;
  }

 public:
  void Create(u64 size_, const void* address_) {
	LinearAllocatorPimpl tmp(size_, address_);
    this->Swap(tmp);
  }

  void Swap(LinearAllocatorPimpl& allocator_) noexcept
  {
    if (this == &allocator_) {
      return;
    }

    std::swap(this->size, allocator_.size);
    std::swap(this->address, allocator_.address);
    std::swap(this->usedSize, allocator_.usedSize);
  }

  void Reset() noexcept {
	LinearAllocatorPimpl tmp {};
    this->Swap(tmp);
  }

 public:
  [[nodiscard]] void* Allocate(u64 size_, u8 alignment_) {
    union {
      void* asVoidPtr;
      uiptr asUintPtr;
    };

    asVoidPtr = (void*)this->address;
	asUintPtr += this->usedSize;

    auto const adjustment = utils::GetAdjustment(size_, alignment_);

    if (!this->IsAllocationPossible(size + adjustment)) {
      return nullptr;
    }

    asUintPtr += adjustment;

    this->usedSize += size + adjustment;

    return asVoidPtr;
  }

  void Free() {
    assert(false &&
           "Linear allocators doesn't support freeing of memory blocks. Call "
           "Clear() instead.");
  }

  void Clear() noexcept { this->usedSize = 0; }

 public:
  [[nodiscard]] inline void const* GetAddress() const { return this->address; }

  [[nodiscard]] inline u64 GetSize() const { return this->size; }

  [[nodiscard]] inline u64 GetUsedSize() const { return this->usedSize; }

 public:
  bool IsAllocationPossible(u64 size_) {
    return this->size >= this->usedSize + size;
  }

 private:
  u64 size;
  const void* address;
  u64 usedSize;
};

LinearAllocator::LinearAllocator()
{
  static_assert(sizeof(pimpl) >= sizeof(LinearAllocatorPimpl),
                "buffer not big enough to hold LinearAllocatorPimpl");

  static_assert(std::alignment_of<AlignedStorageType>::value >= std::alignment_of<LinearAllocatorPimpl>::value,
                "alignment requirements of LinearAllocatorPimpl not fulfilled");

  placement_new<LinearAllocatorPimpl>(&(this->pimpl), sizeof(this->pimpl));
}

LinearAllocator::LinearAllocator(u64 size_, const void* address_)
{
  static_assert(sizeof(pimpl) >= sizeof(LinearAllocatorPimpl),
                "buffer not big enough to hold LinearAllocatorPimpl");

  static_assert(std::alignment_of<AlignedStorageType>::value >= std::alignment_of<LinearAllocatorPimpl>::value,
                "alignment requirements of LinearAllocatorPimpl not fulfilled");

  placement_new<LinearAllocatorPimpl>(&(this->pimpl), sizeof(this->pimpl), size_, address_);
}

LinearAllocator::LinearAllocator(LinearAllocator&& allocator_) noexcept = default;

LinearAllocator& LinearAllocator::operator=(LinearAllocator&& allocator_) noexcept = default;

LinearAllocator::~LinearAllocator()
{
  placement_delete<LinearAllocatorPimpl>(&this->pimpl);
}

void LinearAllocator::Create(u64 size_, const void* address_)
{
  placement_cast<LinearAllocatorPimpl>(&this->pimpl)->Create(size_, address_);
}

void LinearAllocator::Swap(LinearAllocator& allocator_) noexcept
{
  auto allocator_pimpl = placement_cast<LinearAllocatorPimpl>(&allocator_.pimpl);
  placement_cast<LinearAllocatorPimpl>(&this->pimpl)->Swap(*allocator_pimpl);
}

void LinearAllocator::Reset() noexcept
{
  placement_cast<LinearAllocatorPimpl>(&this->pimpl)->Reset();
}

void* LinearAllocator::Allocate(u64 size_, u8 alignment_)
{
  return placement_cast<LinearAllocatorPimpl>(&this->pimpl)->Allocate(size_, alignment_);
}

void LinearAllocator::Free()
{
  placement_cast<LinearAllocatorPimpl>(&this->pimpl)->Free();
}

void LinearAllocator::Clear() noexcept
{
  placement_cast<LinearAllocatorPimpl>(&this->pimpl)->Clear();
}

bool LinearAllocator::IsAllocationPossible(u64 size_)
{
  return placement_cast<LinearAllocatorPimpl>(&this->pimpl)->IsAllocationPossible(size_);
}

[[nodiscard]] void const* LinearAllocator::GetAddress() const
{
  return placement_cast<LinearAllocatorPimpl>(&this->pimpl)->GetAddress();
}

[[nodiscard]] u64 LinearAllocator::GetSize() const
{
  return placement_cast<LinearAllocatorPimpl>(&this->pimpl)->GetSize();
}

[[nodiscard]] u64 LinearAllocator::GetUsedSize() const
{
  return placement_cast<LinearAllocatorPimpl>(&this->pimpl)->GetUsedSize();
}
