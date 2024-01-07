#include "render/texture_config.hpp"

using namespace raptor_engine::structs;

class texture_config::texture_config_pimpl
{
public:
	texture_config_pimpl() 
		: texture_name {}
		, internal_format {}
		, format {}
	{

	}

	texture_config_pimpl(const std::string& texture_name_, i32 internal_format_, i32 format_) 
		: texture_name {texture_name_}
		, internal_format {internal_format_}
		, format {format_}
	{

	}

	~texture_config_pimpl() = default;

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

texture_config::texture_config() 
	: pimpl {std::make_unique<texture_config_pimpl>()} 
{

}

texture_config::texture_config(const std::string& texture_name_, 
	                           i32 internal_format_, 
	                           i32 format_) 
	: pimpl {std::make_unique<texture_config_pimpl>(texture_name_, internal_format_, format_)}
{

}

texture_config::~texture_config() = default;

const std::string texture_config::get_texture_name() const
{
	return this->pimpl->get_texture_name();
}

i32 texture_config::get_internal_format() const
{
	return this->pimpl->get_internal_format();
}

i32 texture_config::get_format() const
{
	return this->pimpl->get_format();
}