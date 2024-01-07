#include "hardware_system_data.hpp"

using namespace raptor_engine::structs;

hardware_system_data::hardware_system_data()
    : sdl_init_flags{}, window_info{std::make_shared<window_data>()} {}

hardware_system_data::hardware_system_data(u32 sdl_init_flags,
                                           const window_data_sptr& window_info)
    : sdl_init_flags{sdl_init_flags}, window_info{window_info} {}

const window_data_sptr hardware_system_data::get_window_data() const
{
	return this->window_info;
}

u32 hardware_system_data::get_init_flags() const
{
	return this->sdl_init_flags;
}