#pragma once

#include "structs/hardware_system_data.hpp"

namespace raptor_engine {
namespace structs {

class engine_data {
 public:
  engine_data();
  engine_data(const hardware_system_data_sptr& hardware_system_info /*=
                  std::make_shared<hardware_system_data>()*/);
  ~engine_data();

 public:
  hardware_system_data_sptr hardware_system_info;
};

using engine_data_uptr = std::unique_ptr<engine_data>;
using engine_data_sptr = std::shared_ptr<engine_data>;
using engine_data_wptr = std::weak_ptr<engine_data>;

}  // namespace structs
}  // namespace raptor_engine