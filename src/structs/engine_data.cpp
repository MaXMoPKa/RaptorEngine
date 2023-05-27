#include "engine_data.hpp"

using namespace raptor_engine::structs;

engine_data::engine_data()
    : hardware_system_info{std::make_shared<hardware_system_data>()} {}

engine_data::engine_data(const hardware_system_data_sptr& hardware_system_info)
    : hardware_system_info{hardware_system_info} {}

engine_data::~engine_data() = default;