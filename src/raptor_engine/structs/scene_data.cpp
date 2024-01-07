#include "structs/scene_data.hpp"

using namespace raptor_engine;
using namespace raptor_engine::structs;

class scene_data::scene_data_pimpl
{
public:
	scene_data_pimpl() 
		: objects {}
		, draw_config {} 
	{
	
	}

	scene_data_pimpl(const render_objects& render_objects_, const draw_config_sptr& draw_config_) 
		: objects {render_objects_}
		, draw_config {draw_config_}
	{

	}

	~scene_data_pimpl() = default;

public:
	const render_objects& get_objects() const
	{
		return this->objects;
	}

	const draw_config_sptr get_draw_config() const
	{
		return this->draw_config;
	}

private:
	render_objects objects;

	draw_config_sptr draw_config;
};

scene_data::scene_data() 
	: pimpl {std::make_unique<scene_data_pimpl>()}
	{

}

scene_data::scene_data(const render_objects& render_objects_, const draw_config_sptr& draw_config_)
	: pimpl {std::make_unique<scene_data_pimpl>(render_objects_, draw_config_)}
{

}

scene_data::~scene_data() = default;

const render_objects& scene_data::get_objects() const
{
	return this->pimpl->get_objects();
}

const draw_config_sptr scene_data::get_draw_config() const
{
	return this->pimpl->get_draw_config();
}