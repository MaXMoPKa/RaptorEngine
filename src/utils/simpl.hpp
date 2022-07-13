#include <cassert>
#include <cstddef>
#include <memory>

namespace raptor_engine {
namespace utils {

/// create an object of type T at a given address
template <typename type, typename... Args>
void placement_new(void *buffer, std::size_t buffer_size, Args... args) {
  // check the size of the buffer (at runtime, in debug builds)
  assert(sizeof(type) <= buffer_size);
  // check the alignment of the buffer (at runtime, in debug builds)
  assert(std::align(std::alignment_of<type>::value, sizeof(type), buffer,
                    buffer_size) == buffer);
  new (buffer) type(args...);
}

/// cast a given address to a pointer to type T
template <typename type>
type *placement_cast(void *buffer) {
  return reinterpret_cast<type *>(buffer);
}
// cast a given address to a const pointer to type T
template <typename type>
type const *placement_cast(void const *buffer) {
  return reinterpret_cast<type const *>(buffer);
}

/// call the destructor of type T at a given address
template <typename type>
void placement_delete(void *buffer) {
  placement_cast<type>(buffer)->~type();
}

}  // namespace utils
}  // namespace raptor_engine