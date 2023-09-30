#pragma once

#include "glad/glad.h"

#include <utility>
#include <memory>

namespace raptor_engine {
namespace render {

struct draw_config
{
public:

	draw_config() : mode {GL_TRIANGLES}, count {0}, type {GL_UNSIGNED_INT}, indices {nullptr}, wireframe {false}
	{ }

	draw_config(unsigned int mode_,
				int			 count_, 
		 		unsigned int type_, 
				void*		 indices_,
				bool         wireframe_) 
		: mode {mode_}, count {count_}, type {type_}, indices {indices_}, wireframe {wireframe_}
	{ }

	~draw_config() = default;

public:

	unsigned int mode;
	int		     count;
	unsigned int type; 
	void*	     indices;
	bool		 wireframe;
};

using draw_config_uptr = std::unique_ptr<draw_config>;
using draw_config_sptr = std::shared_ptr<draw_config>;
using draw_config_wptr = std::weak_ptr<draw_config>;

}
}