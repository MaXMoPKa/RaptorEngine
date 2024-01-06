#pragma once

#include <string>

#include "defines.hpp"

namespace raptor_engine
{
namespace structs
{

struct texture_config
{
public:
	texture_config();

	texture_config(const std::string& texture_name_, i32 internal_format_, i32 format_);

	~texture_config();

public:
	const std::string get_texture_name() const;

	i32 get_internal_format() const;

	i32 get_format() const;

private:
	class texture_config_pimpl;
	std::unique_ptr<texture_config_pimpl> pimpl;

};

using texture_config_uptr = std::unique_ptr<texture_config>;
using texture_config_sptr = std::shared_ptr<texture_config>;
using texture_config_wptr = std::weak_ptr<texture_config>;

} // namespace render
} // namespace raptor_engine