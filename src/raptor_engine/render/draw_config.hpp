#pragma once

#include "glad/glad.h"

#include "defines.hpp"

#include <utility>
#include <memory>

namespace raptor_engine {
namespace structs {

struct draw_config
{
public:

	draw_config();

	draw_config(i32 mode_, i32 count_, i32 type_, void* indices_, bool wireframe_);

	~draw_config();

public:
	i32 get_mode() const;

	i32 get_count() const;

	i32 get_type() const;

	void* get_indices() const;

	bool get_wireframe() const;

private:
	class draw_config_pimpl;
	std::unique_ptr<draw_config_pimpl> pimpl;
};

using draw_config_uptr = std::unique_ptr<draw_config>;
using draw_config_sptr = std::shared_ptr<draw_config>;
using draw_config_wptr = std::weak_ptr<draw_config>;

}
}