#include "init_engine_data.hpp"

using namespace raptor_engine::structs;

init_engine_data::init_engine_data() 
	: hardware_system_info {std::make_shared<hardware_system_data>()}, 
	  scene_info {std::make_shared<scene_data>()}
{ }

init_engine_data::init_engine_data(const hardware_system_data_sptr& hardware_system_info_,
								   const scene_data_sptr&			scene_info_)
    : hardware_system_info{hardware_system_info_}, scene_info {scene_info_}
{ }

init_engine_data::~init_engine_data() = default;

const hardware_system_data_sptr init_engine_data::get_hardware_system_info() const
{
	return this->hardware_system_info;
}

const scene_data_sptr init_engine_data::get_scene_info() const
{
	return this->scene_info;
}