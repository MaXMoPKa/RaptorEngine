#include "render/shader_program.hpp"

using namespace raptor_engine::render;

class shader_program::shader_program_pimpl
{
public:
	shader_program_pimpl() : id {0}, g_uniforms {} { }

	shader_program_pimpl(u32 id_, const std::vector<u32>& g_uniforms_) : id {id_}, g_uniforms {g_uniforms_} { }

public:
	u32 get_id() const
	{
		return this->id;
	}
	void set_id(u32 id_)
	{
		this->id = id_;
	}

	const std::vector<u32>& get_g_uniforms() const
	{
		return this->g_uniforms;
	}
	void set_g_uniforms(const std::vector<u32>& g_uniforms_)
	{
		this->g_uniforms = g_uniforms_;
	}

private:
	u32				 id;
	std::vector<u32> g_uniforms;
};

shader_program::shader_program() 
	: pimpl {std::make_unique<shader_program_pimpl>()} 
{ }

shader_program::shader_program(u32					   id_,
							   const std::vector<u32>& g_uniforms_) 
	: pimpl {std::make_unique<shader_program_pimpl>(id_, g_uniforms_)}
{ }

shader_program::shader_program(shader_program&& program_) noexcept = default;

shader_program& shader_program::operator=(shader_program&& program_) noexcept = default;

shader_program::~shader_program() = default;

u32 shader_program::get_id() const
{
	return this->pimpl->get_id();
}

const std::vector<u32>& shader_program::get_g_uniforms() const
{
	return this->pimpl->get_g_uniforms();
}

void shader_program::set_id(u32 id_) 
{
	this->pimpl->set_id(id_);
}

void shader_program::set_g_uniforms(const std::vector<u32>& g_uniforms_) 
{
	this->pimpl->set_g_uniforms(g_uniforms_);
}
