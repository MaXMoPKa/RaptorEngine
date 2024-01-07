# include "render/texture_program.hpp"

using namespace raptor_engine::render;

class texture_program::texture_program_pimpl
{
public:
	texture_program_pimpl() 
		: texture_id {0} 
	{
	
	}

	texture_program_pimpl(u32 texture_id_) 
		: texture_id {texture_id_} 
	{
	
	}

	~texture_program_pimpl() = default;

public:
	u32 get_id() const
	{
		return this->texture_id;
	}

private:
	u32 texture_id;
};

texture_program::texture_program() 
    : pimpl {std::make_unique<texture_program_pimpl>()} 
{

}

texture_program::texture_program(u32 texture_id_) 
    : pimpl {std::make_unique<texture_program_pimpl>(texture_id_)} 
{

}

texture_program::texture_program(texture_program&& program_) noexcept            = default;
texture_program& texture_program::operator=(texture_program&& program_) noexcept = default;

texture_program::~texture_program() = default;

u32 texture_program::get_id() const
{
	return this->pimpl->get_id();
}