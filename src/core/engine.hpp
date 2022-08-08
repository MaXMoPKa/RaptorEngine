#pragma once

#include <memory>

#include "utils/types.hpp"

using namespace raptor_engine::types;

namespace raptor_engine {

class engine {
 public:
  engine() : hardware_sys(std::make_shared<hardware_system>()) {}

 public:
  hardware_system_sptr get_hardware_system() { return this->hardware_sys; }

 private:
  hardware_system_sptr hardware_sys{};
};

}  // namespace raptor_engine