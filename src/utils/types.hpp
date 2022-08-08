#include <memory>

#include "systems/hardware_system.hpp"

using namespace raptor_enigne::systems;

namespace raptor_engine {
namespace types {

using hardware_system_uptr = std::unique_ptr<hardware_system>;
using hardware_system_sptr = std::shared_ptr<hardware_system>;
using hardware_system_wptr = std::weak_ptr<hardware_system>;

}  // namespace types
}  // namespace raptor_engine