#pragma once

#include <vector>

#include "defines.hpp"

namespace raptor_engine
{
namespace render
{

class shader_program
{
public:

	shader_program();

	shader_program(u32 id_, const std::vector<u32>& g_uniforms_);

	shader_program(shader_program&& program_) noexcept;
	shader_program& operator=(shader_program&& program_) noexcept;

	shader_program(const shader_program&)			 = delete;
	shader_program& operator=(const shader_program&) = delete;

	~shader_program();

public:
	u32 get_id() const;

	const std::vector<u32>& get_g_uniforms() const;

private:
	void set_id(u32 id_);

	void set_g_uniforms(const std::vector<u32>& g_uniforms_);

private:
	class shader_program_pimpl;
	std::unique_ptr<shader_program_pimpl> pimpl;
};

using shader_program_uptr = std::unique_ptr<shader_program>;
using shader_program_sptr = std::shared_ptr<shader_program>;
using shader_program_wptr = std::weak_ptr<shader_program>;

} // namespace render
} // namespace raptor_engine