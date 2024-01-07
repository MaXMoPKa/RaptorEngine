#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "render/texture_program.hpp"
#include "render/texture_config.hpp"

namespace raptor_engine
{
namespace render
{

class texture_manager
{
public:
	texture_manager();

	texture_manager(texture_manager&& manager_) noexcept;
	texture_manager& operator=(texture_manager&& manager_) noexcept;

	texture_manager(const texture_manager&)			   = delete;
	texture_manager& operator=(const texture_manager&) = delete;

	~texture_manager();

public:

	texture_program_sptr add_texture(const structs::texture_config_sptr& texture_);

public:
	#if TESTS
	    bool get_is_texture_file_opened() const;
	#endif


public:
	const std::vector<texture_program_sptr>& get_textures() const;

private:
	class texture_manager_pimpl;
	std::unique_ptr<texture_manager_pimpl> pimpl;
};

using texture_manager_uptr = std::unique_ptr<texture_manager>;
using texture_manager_sptr = std::shared_ptr<texture_manager>;
using texture_manager_wptr = std::weak_ptr<texture_manager>;

} // namespace render
} // namespace raptor_engine