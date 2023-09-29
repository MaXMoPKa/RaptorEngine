#include "hardware_system_data.hpp"

using namespace raptor_engine::structs;

hardware_system_data::hardware_system_data()
    : sdl_init_flags{}, window_info{std::make_shared<window_data>()} {}

hardware_system_data::hardware_system_data(u32 sdl_init_flags,
                                           const window_data_sptr& window_info)
    : sdl_init_flags{sdl_init_flags}, window_info{window_info} {}

void swap(hardware_system_data_sptr& first_data,
          hardware_system_data_sptr& second_data) 
{
  std::swap(first_data->sdl_init_flags, second_data->sdl_init_flags);
  std::swap(first_data->window_info, second_data->window_info);
}