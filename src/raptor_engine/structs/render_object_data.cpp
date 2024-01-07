#include "structs/render_object_data.hpp"

using namespace raptor_engine;
using namespace raptor_engine::structs;

class render_object_data::render_object_data_pimpl
{
public:
	render_object_data_pimpl() 
		: vs_path {}
		, fs_path {}
		, vertices {}
		, indices {}
		, textures {} 
	{
	
	}

	render_object_data_pimpl(const std::string&				  vs_path_,
							 const std::string&				  fs_path_,
							 const std::vector<float>&		  vertices_,
							 const std::vector<unsigned int>& indices_,
							 const std::vector<texture_config_sptr>& textures_)
		: vs_path {vs_path_}
		, fs_path {fs_path_}
		, vertices {vertices_}
		, indices {indices_}
		, textures {textures_}
	{
	
	}

	~render_object_data_pimpl() = default;

public:
	const std::string& get_vs_path() const
	{
		return this->vs_path;
	}

	const std::string& get_fs_path() const
	{
		return this->fs_path;
	}

	const std::vector<float>& get_vertices() const
	{
		return this->vertices;
	}

	const std::vector<u32>& get_indices() const
	{
		return this->indices;
	}

	const std::vector<texture_config_sptr>& get_textures() const
	{
		return this->textures;
	}

private:
	std::string						 vs_path;
	std::string						 fs_path;
	std::vector<float>				 vertices;
	std::vector<u32>				 indices;
	std::vector<texture_config_sptr> textures;
};

render_object_data::render_object_data() 
	: pimpl {std::make_unique<render_object_data_pimpl>()} 
{

}

render_object_data::render_object_data(const std::string& vs_path_,
									   const std::string& fs_path_,
									   const std::vector<float>& vertices_,
									   const std::vector<unsigned int>& indices_,
									   const std::vector<texture_config_sptr>& textures_)
	: pimpl {std::make_unique<render_object_data_pimpl>(vs_path_,
		                                                fs_path_,
		                                                vertices_,
		                                                indices_, 
		                                                textures_)}
{

}

render_object_data::~render_object_data() = default;

const std::string& render_object_data::get_vs_path() const
{
	return this->pimpl->get_vs_path();
}

const std::string& render_object_data::get_fs_path() const 
{
	return this->pimpl->get_fs_path();
}

const std::vector<float>& render_object_data::get_vertices() const 
{
	return this->pimpl->get_vertices();
}

const std::vector<u32>& render_object_data::get_indices() const 
{
	return this->pimpl->get_indices();
}

const std::vector<texture_config_sptr>& render_object_data::get_textures() const 
{
	return this->pimpl->get_textures();
}