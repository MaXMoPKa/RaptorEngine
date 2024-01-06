#pragma once

#include "defines.hpp"

namespace raptor_engine
{
namespace render
{

class texture_program
{
public:
	texture_program();

	texture_program(u32 texture_id_);

	texture_program(texture_program&& program_) noexcept;
	texture_program& operator=(texture_program&& program_) noexcept;

	texture_program(const texture_program&)			   = delete;
	texture_program& operator=(const texture_program&) = delete;

	~texture_program();

public:
	u32 get_id() const;

private:
	class texture_program_pimpl;
	std::unique_ptr<texture_program_pimpl> pimpl;
};

using texture_program_uptr = std::unique_ptr<texture_program>;
using texture_program_sptr = std::shared_ptr<texture_program>;
using texture_program_wptr = std::weak_ptr<texture_program>;

} // namespace render
} // namespace raptor_engine