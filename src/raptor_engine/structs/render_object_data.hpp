#pragma once

#include <vector>
#include <memory>
#include <string>

#include "render/texture_config.hpp"

namespace raptor_engine {
namespace structs {

struct render_object_data
{
public:
	render_object_data();

	render_object_data(const std::string&				path_vs_,
					   const std::string&				path_fs_,
					   const std::vector<float>&		vertices_,
					   const std::vector<unsigned int>& indices_,
					   const std::vector<texture_config_sptr>& textures_ = std::vector<texture_config_sptr> {});
	
	~render_object_data();

public:
	const std::string& get_vs_path() const;

	const std::string& get_fs_path() const;

	const std::vector<float>& get_vertices() const;

	const std::vector<u32>& get_indices() const;

	const std::vector<texture_config_sptr>& get_textures() const;

private:
	std::string vs_path;
	std::string fs_path;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<texture_config_sptr> textures;
};

using render_object_data_uptr = std::unique_ptr<render_object_data>;
using render_object_data_sptr = std::shared_ptr<render_object_data>;
using render_object_data_wptr = std::weak_ptr<render_object_data>;

using render_objects = std::vector<render_object_data>;

}  // namespace structs
}  // namespace raptor_engine