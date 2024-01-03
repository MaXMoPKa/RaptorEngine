#include "structs/render_object_data.hpp"

using namespace raptor_engine;
using namespace raptor_engine::structs;

render_object_data::render_object_data() 
	: vs_path {}
	, fs_path {}
	, vertices {}
	, indices {}
	, textures {} 
{

}

render_object_data::render_object_data(const std::string& vs_path_,
									   const std::string& fs_path_,
									   const std::vector<float>& vertices_,
									   const std::vector<unsigned int>& indices_,
	                                   const std::vector<std::string>& textures_)
	: vs_path {vs_path_}
	, fs_path {fs_path_}
	, vertices {vertices_}
	, indices {indices_}
	, textures {textures_}
{

}

render_object_data::~render_object_data() = default;