#include "base_hardware_system_data.hpp"

using namespace raptor_engine::structs;

base_hardware_system_data::base_hardware_system_data()
    : init_flags{}, window_info{std::make_shared<window_data>()} {}

base_hardware_system_data::base_hardware_system_data(u32 init_flags,
                                                     const window_data_sptr& window_info)
    : init_flags{init_flags}, window_info{window_info} {}

void swap(base_hardware_system_data_sptr& first_data, base_hardware_system_data_sptr& second_data)
{
  std::swap(first_data->init_flags, second_data->init_flags);
  std::swap(first_data->window_info, second_data->window_info);
}