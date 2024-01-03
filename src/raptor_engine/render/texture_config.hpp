#pragma once

#include <string>

#include "defines.hpp"

namespace raptor_engine
{
namespace structs
{

class texture_config
{
public:
	texture_config() 
		: texture_name {}
		, internal_format {}
		, format {}
	{

	}

	texture_config(const std::string& texture_name_, 
		           i32 internal_format_, 
		           i32 format_) 
		: texture_name {texture_name_}
		, internal_format {internal_format_}
		, format {format_}
	{

	}

public:
	const std::string get_texture_name() const
	{
		return this->texture_name;
	}

	i32 get_internal_format() const
	{
		return this->internal_format;
	}

	i32 get_format() const
	{
		return this->format;
	}

private:
	std::string texture_name;
	i32			internal_format;
	i32			format;

};

using texture_config_uptr = std::unique_ptr<texture_config>;
using texture_config_sptr = std::shared_ptr<texture_config>;
using texture_config_wptr = std::weak_ptr<texture_config>;

} // namespace render
} // namespace raptor_engine