#include "sdl_hardware_system_data.hpp"

using namespace raptor_engine::structs;

sdl_hardware_system_data::sdl_hardware_system_data()
	: base_hardware_system_data {{}, std::make_shared<window_data>()} { }

sdl_hardware_system_data::sdl_hardware_system_data(u32 sdl_init_flags,
                                                   const window_data_sptr& window_info)
	: base_hardware_system_data {sdl_init_flags, window_info}
{ }

void swap(sdl_hardware_system_data_sptr& first_data, sdl_hardware_system_data_sptr& second_data)
{
	std::swap(first_data, second_data);
}